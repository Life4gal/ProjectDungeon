// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/room.hpp>

#include <algorithm>
#include <ranges>

#include <config/collision_mask.hpp>
#include <config/dungeon.hpp>

#include <components/room.hpp>

#include <systems/helper/world.hpp>
#include <systems/helper/physics_world.hpp>
#include <systems/helper/transform.hpp>
#include <systems/helper/render.hpp>
#include <systems/helper/animation.hpp>
#include <systems/helper/physics_body.hpp>

#include <game/log_entity.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::systems::helper
{
	auto Room::create(
		entt::registry& registry,
		const config::Room& room,
		// config::Dungeon::animation_set
		const config::AnimationSet& animation_set,
		// config::Dungeon::tile_set
		const config::TileSet& tile_set
	) noexcept -> entt::entity
	{
		using namespace components;

		const auto entity = registry.create();

		// room
		registry.emplace<room::Room>(entity, std::cref(room));

		// 计算房间偏移,使房间居中显示
		const auto room_width = static_cast<float>(room.width * room.tile_size);
		const auto room_height = static_cast<float>(room.height * room.tile_size);
		const auto room_offset_x = (static_cast<float>(World::window_width(registry)) - room_width) * 0.5f;
		const auto room_offset_y = (static_cast<float>(World::window_height(registry)) - room_height) * 0.5f;

		// tiles
		{
			const auto room_tile_size = static_cast<float>(room.tile_size);
			const auto room_half_tile_size = room_tile_size * 0.5f;

			const auto do_create_tile = [&]<typename PositionToDataMapping, typename TileIdToTileMapping>(
				const std::string_view type,
				// map: position --> T
				const PositionToDataMapping& tiles,
				// map: tile_id --> XxxTile
				const TileIdToTileMapping& set,
				// T + XxxTile + tile-position --> entity
				auto tile_entity_maker,
				// entity container
				auto& entity_container
			) noexcept -> void //
						requires requires
						{
							{
								tile_entity_maker(
									typename PositionToDataMapping::mapped_type{},
									typename TileIdToTileMapping::mapped_type{},
									sf::Vector2f{}
								)
							} -> std::same_as<entt::entity>;
						}
			{
				entity_container.reserve(tiles.size());

				for (const auto& [position, data]: tiles)
				{
					// config::Wall::tile_id
					// config::Floor::tile_id
					// config::Decoration::tile_id
					// config::Trigger::tile_id
					// config::Door::tile_id
					const auto& tile_id = data.tile_id;
					const auto tile_it = set.find(tile_id);
					if (tile_it == set.end())
					{
						SPDLOG_WARN("房间[{}]引用了不存在的{}瓦片[{}]", room.name, type, tile_id);
						continue;
					}

					const auto& tile = tile_it->second;

					const auto tile_x = room_offset_x + static_cast<float>(position.x) * room_tile_size + room_half_tile_size;
					const auto tile_y = room_offset_y + static_cast<float>(position.y) * room_tile_size + room_half_tile_size;
					const auto tile_position = sf::Vector2f{tile_x, tile_y};

					const auto tile_entity = tile_entity_maker(data, tile, tile_position);
					entity_container.emplace_back(tile_entity);
				}
			};

			// walls
			{
				const auto& tiles = room.wall_tiles;
				auto& [walls] = registry.emplace<room::Walls>(entity);

				do_create_tile(
					"墙壁",
					tiles,
					tile_set.wall_tiles,
					[&](const config::Wall& wall, const config::WallTile& tile, const sf::Vector2f position) noexcept -> entt::entity
					{
						return create_wall(registry, wall, tile, animation_set, position);
					},
					walls
				);
			}
			// floors
			{
				const auto& tiles = room.floor_tiles;
				auto& [floors] = registry.emplace<room::Floors>(entity);

				do_create_tile(
					"地板",
					tiles,
					tile_set.floor_tiles,
					[&](const config::Floor& floor, const config::FloorTile& tile, const sf::Vector2f position) noexcept -> entt::entity
					{
						return create_floor(registry, floor, tile, animation_set, position);
					},
					floors
				);
			}
			// decorations
			{
				const auto& tiles = room.decoration_tiles;
				auto& [decorations] = registry.emplace<room::Decorations>(entity);

				do_create_tile(
					"装饰物",
					tiles,
					tile_set.decoration_tiles,
					[&](const config::Decoration& decoration, const config::DecorationTile& tile, const sf::Vector2f position) noexcept -> entt::entity
					{
						return create_decoration(registry, decoration, tile, animation_set, position);
					},
					decorations
				);
			}
			// triggers
			{
				const auto& tiles = room.trigger_tiles;
				auto& [triggers] = registry.emplace<room::Triggers>(entity);

				do_create_tile(
					"触发器",
					tiles,
					tile_set.trigger_tiles,
					[&](const config::Trigger& trigger, const config::TriggerTile& tile, const sf::Vector2f position) noexcept -> entt::entity
					{
						return create_trigger(registry, trigger, tile, animation_set, position);
					},
					triggers
				);
			}
			// rooms
			{
				const auto& tiles = room.door_tiles;
				auto& [doors] = registry.emplace<room::Doors>(entity);

				do_create_tile(
					"门",
					tiles,
					tile_set.door_tiles,
					[&](const config::Door& door, const config::DoorTile& tile, const sf::Vector2f position) noexcept -> entt::entity
					{
						if (room.type == config::RoomType::STARTING)
						{
							return create_unlocked_door(registry, door, tile, animation_set, position);
						}

						return create_locked_door(registry, door, tile, animation_set, position);
					},
					doors
				);
			}
		}

		// enemies
		registry.emplace<room::Enemies>(entity);

		// offset
		registry.emplace<room::CenteringOffset>(entity, room_offset_x, room_offset_y);

		SPDLOG_INFO("生成房间[{}]成功!", room.name);

		return entity;
	}

	auto Room::destroy(entt::registry& registry, const entt::entity room_entity) noexcept -> void
	{
		using namespace components;

		const auto destroy_entities = [&]<typename T>(auto destroyer) noexcept -> void //
					requires requires { destroyer(registry, entt::entity{}); }
		{
			const auto& [entities] = registry.get<T>(room_entity);

			std::ranges::for_each(
				entities,
				[&](const entt::entity e) noexcept -> void
				{
					if (registry.valid(e))
					{
						destroyer(registry, e);
					}
				}
			);
		};

		// walls
		destroy_entities.operator()<room::Walls>(&destroy_wall);
		// floors
		destroy_entities.operator()<room::Floors>(&destroy_floor);
		// decorations
		destroy_entities.operator()<room::Decorations>(&destroy_decoration);
		// triggers
		destroy_entities.operator()<room::Triggers>(&destroy_trigger);
		// doors
		destroy_entities.operator()<room::Doors>(&destroy_door);

		// todo: Enemies需不需要处理?

		const auto& room = registry.get<const room::Room>(room_entity).room.get();
		SPDLOG_INFO("销毁房间[{}]完成", room.name);

		registry.destroy(room_entity);
	}

	// ReSharper disable once CppParameterMayBeConstPtrOrRef
	auto Room::is_cleared(entt::registry& registry, const entt::entity room_entity) noexcept -> bool
	{
		using namespace components;

		return registry.all_of<room::Cleared>(room_entity);
	}

	auto Room::update_enemy_entities(entt::registry& registry, const entt::entity room_entity) noexcept -> bool
	{
		using namespace components;

		auto& [enemy_entities] = registry.get<room::Enemies>(room_entity);
		if (enemy_entities.empty())
		{
			return true;
		}

		const auto invalid_r = std::ranges::remove_if(
			enemy_entities,
			[&registry](const entt::entity e) noexcept -> bool
			{
				return not registry.valid(e);
			}
		);

		enemy_entities.erase(invalid_r.begin(), enemy_entities.end());

		return enemy_entities.empty();
	}

	auto Room::unlock_doors(entt::registry& registry, const entt::entity room_entity) noexcept -> void
	{
		using namespace components;

		auto& [doors] = registry.get<room::Doors>(room_entity);

		std::ranges::for_each(
			doors,
			[&registry](const entt::entity door_entity) noexcept -> void
			{
				if (registry.valid(door_entity))
				{
					unlock_door(registry, door_entity);
				}
			}
		);
	}

	auto Room::create_wall(
		entt::registry& registry,
		const config::Wall& wall,
		const config::WallTile& wall_tile,
		const config::AnimationSet& animation_set,
		const sf::Vector2f position,
		const sf::Vector2f scale,
		const sf::Angle rotation
	) noexcept -> entt::entity
	{
		using namespace components;

		const auto animation_it = animation_set.find(wall_tile.animation_id);
		if (animation_it == animation_set.end())
		{
			SPDLOG_ERROR("找不到墙壁[{}]的动画[{}]!", wall.tile_id, wall_tile.animation_id);
			// todo: 怎么办?有没有备用方案?
			return entt::null;
		}
		const auto& animation = animation_it->second;
		const auto& first_frame = animation.frames.front();

		const auto entity = registry.create();

		const auto texture_width = first_frame.texture_width;
		const auto texture_height = first_frame.texture_height;
		const auto size = sf::Vector2f{static_cast<float>(texture_width) * scale.x, static_cast<float>(texture_height) * scale.y};

		auto* physics_user_data = PhysicsWorld::to_user_data(entity);
		const auto physics_position = PhysicsWorld::physics_position_of(position);
		const auto physics_size = PhysicsWorld::physics_size_of(size);
		const auto physics_rotation = PhysicsWorld::physics_rotation_of(rotation);

		// transform
		Transform::attach(registry, entity, position, scale, rotation);
		// render
		Render::attach(registry, entity, first_frame, config::RenderLayer::WALL);
		// animation
		Animation::attach(registry, entity, animation);
		// physics_body
		{
			const auto world_id = PhysicsWorld::id(registry);

			// 创建静态刚体
			auto body_def = b2DefaultBodyDef();
			body_def.type = b2_staticBody;
			body_def.position = physics_position;
			body_def.rotation = physics_rotation;
			body_def.userData = physics_user_data;

			auto shape_def = b2DefaultShapeDef();
			// 设置碰撞过滤
			shape_def.filter.categoryBits = static_cast<std::uint64_t>(config::RenderLayer::WALL);
			shape_def.filter.maskBits = static_cast<std::uint64_t>(config::CollisionMask::WALL);

			// 创建矩形碰撞体
			const auto box = b2MakeBox(physics_size.x / 2, physics_size.y * 2);

			PhysicsBody::attach(registry, entity, world_id, body_def);
			PhysicsBody::attach_shape(registry, entity, shape_def, box);
		}

		log::on_create("墙壁", entity, position, size, rotation, physics_position, physics_size);

		return entity;
	}

	auto Room::destroy_wall(entt::registry& registry, const entt::entity wall_entity) noexcept -> void
	{
		using namespace components;

		// physics_body
		PhysicsBody::deattach(registry, wall_entity);

		log::on_destroy("墙壁", wall_entity);

		registry.destroy(wall_entity);
	}

	auto Room::create_floor(
		entt::registry& registry,
		const config::Floor& floor,
		const config::FloorTile& floor_tile,
		const config::AnimationSet& animation_set,
		const sf::Vector2f position,
		const sf::Vector2f scale,
		const sf::Angle rotation
	) noexcept -> entt::entity
	{
		using namespace components;

		const auto animation_it = animation_set.find(floor_tile.animation_id);
		if (animation_it == animation_set.end())
		{
			SPDLOG_ERROR("找不到地板[{}]的动画[{}]!", floor.tile_id, floor_tile.animation_id);
			// todo: 怎么办?有没有备用方案?
			return entt::null;
		}
		const auto& animation = animation_it->second;
		const auto& first_frame = animation.frames.front();

		const auto entity = registry.create();

		const auto texture_width = first_frame.texture_width;
		const auto texture_height = first_frame.texture_height;
		const auto size = sf::Vector2f{static_cast<float>(texture_width) * scale.x, static_cast<float>(texture_height) * scale.y};

		// transform
		Transform::attach(registry, entity, position, scale, rotation);
		// render
		Render::attach(registry, entity, first_frame, config::RenderLayer::FLOOR);
		// animation
		Animation::attach(registry, entity, animation);

		log::on_create("地板", entity, position, size, rotation);

		return entity;
	}

	auto Room::destroy_floor(entt::registry& registry, const entt::entity floor_entity) noexcept -> void
	{
		using namespace components;

		log::on_destroy("地板", floor_entity);

		registry.destroy(floor_entity);
	}

	auto Room::create_decoration(
		entt::registry& registry,
		const config::Decoration& decoration,
		const config::DecorationTile& decoration_tile,
		const config::AnimationSet& animation_set,
		const sf::Vector2f position,
		const sf::Vector2f scale,
		const sf::Angle rotation
	) noexcept -> entt::entity
	{
		using namespace components;

		const auto animation_it = animation_set.find(decoration_tile.animation_id);
		if (animation_it == animation_set.end())
		{
			SPDLOG_ERROR("找不到装饰物[{}]的动画[{}]!", decoration.tile_id, decoration_tile.animation_id);
			// todo: 怎么办?有没有备用方案?
			return entt::null;
		}
		const auto& animation = animation_it->second;
		const auto& first_frame = animation.frames.front();

		const auto entity = registry.create();

		const auto texture_width = first_frame.texture_width;
		const auto texture_height = first_frame.texture_height;
		const auto size = sf::Vector2f{static_cast<float>(texture_width) * scale.x, static_cast<float>(texture_height) * scale.y};

		// transform
		Transform::attach(registry, entity, position, scale, rotation);
		// render
		Render::attach(registry, entity, first_frame, config::RenderLayer::DECORATION);
		// animation
		Animation::attach(registry, entity, animation);

		log::on_create("装饰物", entity, position, size, rotation);

		return entity;
	}

	auto Room::destroy_decoration(entt::registry& registry, const entt::entity decoration_entity) noexcept -> void
	{
		using namespace components;

		log::on_destroy("装饰物", decoration_entity);

		registry.destroy(decoration_entity);
	}

	auto Room::create_trigger(
		entt::registry& registry,
		const config::Trigger& trigger,
		const config::TriggerTile& trigger_tile,
		const config::AnimationSet& animation_set,
		const sf::Vector2f position,
		const sf::Vector2f scale,
		const sf::Angle rotation
	) noexcept -> entt::entity
	{
		using namespace components;

		const auto animation_it = animation_set.find(trigger_tile.animation_id);
		if (animation_it == animation_set.end())
		{
			SPDLOG_ERROR("找不到触发器[{}]的动画[{}]!", trigger.tile_id, trigger_tile.animation_id);
			// todo: 怎么办?有没有备用方案?
			return entt::null;
		}
		const auto& animation = animation_it->second;
		const auto& first_frame = animation.frames.front();

		const auto entity = registry.create();

		const auto texture_width = first_frame.texture_width;
		const auto texture_height = first_frame.texture_height;
		const auto size = sf::Vector2f{static_cast<float>(texture_width) * scale.x, static_cast<float>(texture_height) * scale.y};

		auto* physics_user_data = PhysicsWorld::to_user_data(entity);
		const auto physics_position = PhysicsWorld::physics_position_of(position);
		const auto physics_size = PhysicsWorld::physics_size_of(size);
		const auto physics_rotation = PhysicsWorld::physics_rotation_of(rotation);

		// transform
		Transform::attach(registry, entity, position, scale, rotation);
		// render
		Render::attach(registry, entity, first_frame, config::RenderLayer::TRIGGER);
		// animation
		Animation::attach(registry, entity, animation);
		// physics_body
		{
			const auto world_id = PhysicsWorld::id(registry);

			// 创建静态刚体
			auto body_def = b2DefaultBodyDef();
			body_def.type = b2_staticBody;
			body_def.position = physics_position;
			body_def.rotation = physics_rotation;
			body_def.fixedRotation = true;
			body_def.userData = physics_user_data;

			auto shape_def = b2DefaultShapeDef();
			// 设置碰撞过滤
			shape_def.filter.categoryBits = static_cast<std::uint64_t>(config::RenderLayer::TRIGGER);
			shape_def.filter.maskBits = static_cast<std::uint64_t>(config::CollisionMask::TRIGGER);
			// 传感器
			if (trigger_tile.is_sensor)
			{
				shape_def.isSensor = true;
				shape_def.enableSensorEvents = true;
			}
			else
			{
				shape_def.enableContactEvents = true;
			}

			// 创建矩形碰撞体
			const auto box = b2MakeBox(physics_size.x / 2, physics_size.y * 2);

			PhysicsBody::attach(registry, entity, world_id, body_def);
			PhysicsBody::attach_shape(registry, entity, shape_def, box);
		}

		log::on_create("触发器", entity, position, size, rotation, physics_position, physics_size);

		return entity;
	}

	auto Room::destroy_trigger(entt::registry& registry, const entt::entity trigger_entity) noexcept -> void
	{
		using namespace components;

		// physics_body
		PhysicsBody::deattach(registry, trigger_entity);

		log::on_destroy("触发器", trigger_entity);

		registry.destroy(trigger_entity);
	}

	namespace
	{
		template<bool Lock>
		auto do_create_door(
			entt::registry& registry,
			const config::Door& door,
			const config::DoorTile& door_tile,
			const config::AnimationSet& animation_set,
			const sf::Vector2f position,
			const sf::Vector2f scale
		) noexcept -> entt::entity
		{
			using namespace components;

			// 根据门的方向设置旋转
			const auto rotation = [&] noexcept -> sf::Angle
			{
				switch (door.direction)
				{
					case config::DoorDirection::UP:
					{
						return sf::degrees(0);
					}
					case config::DoorDirection::DOWN:
					{
						return sf::degrees(180);
					}
					case config::DoorDirection::LEFT:
					{
						return sf::degrees(270);
					}
					case config::DoorDirection::RIGHT:
					{
						return sf::degrees(90);
					}
					default: // NOLINT(clang-diagnostic-covered-switch-default)
					{
						std::unreachable();
					}
				}
			}();

			const auto animation_it = animation_set.find(door_tile.animation_id);
			if (animation_it == animation_set.end())
			{
				SPDLOG_ERROR("找不到门[{}]的动画[{}]!", door.tile_id, door_tile.animation_id);
				// todo: 怎么办?有没有备用方案?
				return entt::null;
			}
			const auto& animation = animation_it->second;
			const auto& first_frame = animation.frames.front();

			const auto entity = registry.create();

			const auto texture_width = first_frame.texture_width;
			const auto texture_height = first_frame.texture_height;
			const auto size = sf::Vector2f{static_cast<float>(texture_width) * scale.x, static_cast<float>(texture_height) * scale.y};

			auto* physics_user_data = PhysicsWorld::to_user_data(entity);
			const auto physics_position = PhysicsWorld::physics_position_of(position);
			const auto physics_size = PhysicsWorld::physics_size_of(size);
			const auto physics_rotation = PhysicsWorld::physics_rotation_of(rotation);

			// transform
			Transform::attach(registry, entity, position, scale, rotation);
			// render
			Render::attach(registry, entity, first_frame, config::RenderLayer::DOOR);
			// animation
			Animation::attach(registry, entity, animation);
			// physics_body
			{
				const auto world_id = PhysicsWorld::id(registry);

				// todo: 根据方向设置触发区域

				// 创建静态刚体
				auto body_def = b2DefaultBodyDef();
				body_def.type = b2_staticBody;
				body_def.position = physics_position;
				body_def.rotation = physics_rotation;
				body_def.fixedRotation = true;
				body_def.userData = physics_user_data;

				auto shape_def = b2DefaultShapeDef();
				// 设置碰撞过滤
				shape_def.filter.categoryBits = static_cast<std::uint64_t>(config::RenderLayer::DOOR);
				if constexpr (Lock)
				{
					shape_def.filter.maskBits = static_cast<std::uint64_t>(config::CollisionMask::DOOR_CLOSE);
				}
				else
				{
					shape_def.filter.maskBits = static_cast<std::uint64_t>(config::CollisionMask::DOOR_OPEN);
					// 传感器
					shape_def.isSensor = true;
					shape_def.enableSensorEvents = true;
				}

				// 创建矩形碰撞体
				const auto box = b2MakeBox(physics_size.x / 2, physics_size.y * 2);

				PhysicsBody::attach(registry, entity, world_id, body_def);
				PhysicsBody::attach_shape(registry, entity, shape_def, box);

				// door
				registry.emplace<door::CollisionSize>(entity, physics_size.x / 2, physics_size.y * 2);
			}
			// door
			{
				if constexpr (Lock)
				{
					registry.emplace<door::Locked>(entity);
				}

				// Key组件由外部调用set_key函数设置,这里不处理
				// 保险起见先添加该组件?
				registry.emplace<door::Key>(entity, entt::null);

				registry.emplace<door::Direction>(entity, door.direction);
				registry.emplace<door::TargetRoom>(entity, door.target_room);
			}

			log::on_create("门", entity, position, size, rotation, physics_position, physics_size);

			return entity;
		}
	}

	auto Room::create_locked_door(
		entt::registry& registry,
		const config::Door& door,
		const config::DoorTile& door_tile,
		const config::AnimationSet& animation_set,
		const sf::Vector2f position,
		const sf::Vector2f scale
	) noexcept -> entt::entity
	{
		using namespace components;

		return do_create_door<true>(registry, door, door_tile, animation_set, position, scale);
	}

	auto Room::create_unlocked_door(
		entt::registry& registry,
		const config::Door& door,
		const config::DoorTile& door_tile,
		const config::AnimationSet& animation_set,
		const sf::Vector2f position,
		const sf::Vector2f scale
	) noexcept -> entt::entity
	{
		using namespace components;

		return do_create_door<false>(registry, door, door_tile, animation_set, position, scale);
	}

	auto Room::destroy_door(entt::registry& registry, const entt::entity door_entity) noexcept -> void
	{
		using namespace components;

		// physics_body
		PhysicsBody::deattach(registry, door_entity);

		log::on_destroy("门", door_entity);

		registry.destroy(door_entity);
	}

	auto Room::set_door_key(entt::registry& registry, const entt::entity door_entity, const entt::entity key_entity) noexcept -> void
	{
		using namespace components;

		registry.emplace_or_replace<door::Key>(door_entity, key_entity);
	}

	auto Room::unlock_door(entt::registry& registry, const entt::entity door_entity) noexcept -> void
	{
		using namespace components;

		// 移除Locked组件
		registry.remove<door::Locked>(door_entity);
		// 移除已有shape
		PhysicsBody::deattach_shape(registry, door_entity);
		// 创建sensor
		{
			auto shape_def = b2DefaultShapeDef();
			shape_def.filter.categoryBits = static_cast<std::uint64_t>(config::RenderLayer::DOOR);
			shape_def.filter.maskBits = static_cast<std::uint64_t>(config::CollisionMask::DOOR_OPEN);
			shape_def.isSensor = true;
			shape_def.enableSensorEvents = true;

			const auto [half_width, half_height] = registry.get<const door::CollisionSize>(door_entity);
			const auto box = b2MakeBox(half_width, half_height);

			PhysicsBody::attach_shape(registry, door_entity, shape_def, box);
		}
	}

	auto Room::lock_door(entt::registry& registry, const entt::entity door_entity) noexcept -> void
	{
		using namespace components;

		// 添加Locked组件
		registry.emplace_or_replace<door::Locked>(door_entity);
		// 移除已有shape
		PhysicsBody::deattach_shape(registry, door_entity);
		// 创建solid shape
		{
			auto shape_def = b2DefaultShapeDef();
			shape_def.filter.categoryBits = static_cast<std::uint64_t>(config::RenderLayer::DOOR);
			shape_def.filter.maskBits = static_cast<std::uint64_t>(config::CollisionMask::DOOR_CLOSE);

			const auto [half_width, half_height] = registry.get<const door::CollisionSize>(door_entity);
			const auto box = b2MakeBox(half_width, half_height);

			PhysicsBody::attach_shape(registry, door_entity, shape_def, box);
		}
	}
}
