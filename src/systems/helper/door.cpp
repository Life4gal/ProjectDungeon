// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/door.hpp>

#include <config/dungeon.hpp>

#include <components/entity/door.hpp>

#include <components/transform.hpp>
#include <components/render.hpp>
#include <components/physics_body.hpp>

#include <systems/helper/physics_world.hpp>

#include <game/constants.hpp>
#include <game/entity_type.hpp>
#include <game/user_data_entity.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::systems::helper
{
	namespace
	{
		template<bool Lock>
		auto do_spawn(
			entt::registry& registry,
			const config::Tile& tile,
			const config::DoorDirection direction,
			const std::string& target_room_id,
			const sf::Vector2f position,
			const sf::Vector2f scale
		) noexcept -> entt::entity
		{
			using namespace components;

			const auto entity = registry.create();

			// transform
			{
				using namespace transform;

				registry.emplace<Position>(entity, position);
				registry.emplace<Scale>(entity, scale);

				// 根据门的方向设置旋转
				const auto rotation = [&] noexcept -> sf::Angle
				{
					switch (direction)
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

				registry.emplace<Rotation>(entity, rotation);
			}
			// render
			{
				using namespace render;

				registry.emplace<Texture>(
					entity,
					Texture
					{
							.texture_path = tile.texture_path,
							.frame_x = tile.texture_x,
							.frame_y = tile.texture_y,
							.frame_width = tile.texture_width,
							.frame_height = tile.texture_height
					}
				);

				registry.emplace<Color>(entity, sf::Color::White);
			}
			// physics
			{
				using namespace physics_body;

				const auto world_id = PhysicsWorld::id(registry);

				// 创建静态刚体
				auto body_def = b2DefaultBodyDef();
				body_def.type = b2_staticBody;
				body_def.position = Constant::to_physics(position);
				body_def.userData = entity_to_user_data(entity);

				const auto body_id = b2CreateBody(world_id, &body_def);

				// 创建矩形碰撞体
				const auto half_width = Constant::to_physics(static_cast<float>(tile.texture_width) * scale.x * 0.5f);
				const auto half_height = Constant::to_physics(static_cast<float>(tile.texture_height) * scale.y * 0.5f);

				auto shape_def = b2DefaultShapeDef();
				// 设置碰撞过滤
				shape_def.filter.categoryBits = static_cast<std::uint64_t>(EntityType::DOOR);
				if constexpr (Lock)
				{
					shape_def.filter.maskBits = CollisionMask::door_locked;
				}
				else
				{
					shape_def.filter.maskBits = CollisionMask::door_unlocked;
					// 传感器
					shape_def.isSensor = true;
					shape_def.enableSensorEvents = true;
				}

				const auto box = b2MakeBox(half_width, half_height);

				b2CreatePolygonShape(body_id, &shape_def, &box);

				registry.emplace<BodyId>(entity, body_id);
			}
			// door
			{
				using namespace door;

				registry.emplace<Direction>(entity, direction);
				registry.emplace<TargetRoom>(entity, target_room_id);

				if constexpr (Lock)
				{
					registry.emplace<Locked>(entity);
				}

				// Key组件由外部调用set_key函数设置,这里不处理
				// 保险起见先添加该组件?
				registry.emplace<Key>(entity, entt::null);

				registry.emplace<Direction>(entity, direction);
				registry.emplace<TargetRoom>(entity, target_room_id);

				const float half_width = Constant::to_physics(static_cast<float>(tile.texture_width) * scale.x * 0.5f);
				const float half_height = Constant::to_physics(static_cast<float>(tile.texture_height) * scale.y * 0.5f);
				registry.emplace<CollisionSize>(entity, half_width, half_height);
			}

			return entity;
		}
	}

	auto Door::spawn_locked(
		entt::registry& registry,
		const config::Tile& tile,
		const config::DoorDirection direction,
		const std::string& target_room_id,
		const sf::Vector2f position,
		const sf::Vector2f scale
	) noexcept -> entt::entity
	{
		return do_spawn<true>(registry, tile, direction, target_room_id, position, scale);
	}

	auto Door::spawn_unlocked(
		entt::registry& registry,
		const config::Tile& tile,
		const config::DoorDirection direction,
		const std::string& target_room_id,
		const sf::Vector2f position,
		const sf::Vector2f scale
	) noexcept -> entt::entity
	{
		return do_spawn<false>(registry, tile, direction, target_room_id, position, scale);
	}

	auto Door::set_key(entt::registry& registry, const entt::entity entity, const entt::entity key_entity) noexcept -> void
	{
		using namespace components;

		registry.emplace_or_replace<door::Key>(entity, key_entity);
	}

	auto Door::unlock(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		using namespace components;

		// 移除Locked组件
		registry.remove<door::Locked>(entity);

		const auto [body_id] = registry.get<const physics_body::BodyId>(entity);
		const auto [half_width, half_height] = registry.get<const door::CollisionSize>(entity);

		// 销毁所有现有shape
		{
			if (const auto count = b2Body_GetShapeCount(body_id);
				count != 0)
			{
				// todo: 我们是否需要确定只有一个shape?理论上应该只有一个shape

				const auto shape_ids = std::make_unique<b2ShapeId[]>(static_cast<std::size_t>(count));
				b2Body_GetShapes(body_id, shape_ids.get(), count);

				for (const auto shape_id: std::views::counted(shape_ids.get(), count))
				{
					b2DestroyShape(shape_id, false);
				}
			}
		}

		// 创建sensor
		{
			auto def = b2DefaultShapeDef();
			def.filter.categoryBits = static_cast<std::uint64_t>(EntityType::DOOR);
			def.filter.maskBits = CollisionMask::door_unlocked;
			def.isSensor = true;
			def.enableSensorEvents = true;

			const auto box = b2MakeBox(half_width, half_height);

			b2CreatePolygonShape(body_id, &def, &box);
		}
	}

	auto Door::lock(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		using namespace components;

		// 添加Locked组件
		registry.emplace_or_replace<door::Locked>(entity);

		const auto [body_id] = registry.get<const physics_body::BodyId>(entity);
		const auto [half_width, half_height] = registry.get<const door::CollisionSize>(entity);

		// 销毁所有现有shape
		{
			if (const auto count = b2Body_GetShapeCount(body_id);
				count != 0)
			{
				// todo: 我们是否需要确定只有一个shape?理论上应该只有一个shape

				const auto shape_ids = std::make_unique<b2ShapeId[]>(static_cast<std::size_t>(count));
				b2Body_GetShapes(body_id, shape_ids.get(), count);

				for (const auto shape_id: std::views::counted(shape_ids.get(), count))
				{
					b2DestroyShape(shape_id, false);
				}
			}
		}

		// 创建solid shape
		{
			auto def = b2DefaultShapeDef();
			def.filter.categoryBits = static_cast<std::uint64_t>(EntityType::DOOR);
			def.filter.maskBits = CollisionMask::door_locked;

			const auto box = b2MakeBox(half_width, half_height);

			b2CreatePolygonShape(body_id, &def, &box);
		}
	}
} // namespace pd::systems::helper
