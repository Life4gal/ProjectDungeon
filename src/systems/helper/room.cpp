// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/room.hpp>

#include <algorithm>
#include <ranges>

#include <config/dungeon.hpp>

#include <components/entity/room.hpp>

#include <components/physics_body.hpp>

#include <systems/helper/floor.hpp>
#include <systems/helper/wall.hpp>
#include <systems/helper/decoration.hpp>
#include <systems/helper/door.hpp>
#include <systems/helper/terrain.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

namespace pd::systems::helper
{
	auto Room::create(
		entt::registry& registry,
		const config::Room& room,
		const config::TileSet& tileset,
		const sf::Vector2f offset
	) noexcept -> entt::entity
	{
		using namespace components;

		const auto entity = registry.create();

		// room
		{
			const auto tile_size = static_cast<float>(room.tile_size);

			registry.emplace<room::Id>(entity, room.id);

			// entities
			{
				auto& [tile_entities] = registry.emplace<room::TileEntities>(entity);

				const auto& layout = room.layout;
				const auto& tile_mapping = room.tile_mapping;

				const auto total_tiles = std::ranges::fold_left(
					layout,
					std::size_t{0},
					[](const std::size_t total, const std::string& line) noexcept -> std::size_t
					{
						return total + line.size();
					}
				);
				tile_entities.reserve(total_tiles);

				for (std::size_t row = 0; row < layout.size(); ++row)
				{
					const auto& line = layout[row];
					for (std::size_t column = 0; column < line.size(); ++column)
					{
						const auto tile_char = line[column];

						const auto mapping_it = tile_mapping.find(tile_char);
						if (mapping_it == tile_mapping.end())
						{
							SPDLOG_WARN("房间[{}]中发现未映射的字符:'{}'", room.id, tile_char);
							continue;
						}

						const auto& tile_id = mapping_it->second;
						const auto tile_it = tileset.find(tile_id);
						if (tile_it == tileset.end())
						{
							SPDLOG_WARN("房间[{}]引用了不存在的瓦片:{}", room.id, tile_id);
							continue;
						}

						const auto& tile = tile_it->second;

						// 计算瓦片位置(中心点)
						// 瓦片从左上角开始排列,位置为瓦片中心
						const auto x = offset.x + static_cast<float>(column) * tile_size + tile_size * 0.5f;
						const auto y = offset.y + static_cast<float>(row) * tile_size + tile_size * 0.5f;
						const auto position = sf::Vector2f{x, y};

						const auto tile_entity = [&] noexcept -> entt::entity
						{
							switch (tile.type)
							{
								case config::TileType::FLOOR:
								{
									return Floor::spawn(registry, tile, position);
								}
								case config::TileType::WALL:
								{
									return Wall::spawn(registry, tile, position);
								}
								case config::TileType::WALL_DECORATION:
								{
									return Decoration::spawn_wall(registry, tile, position);
								}
								case config::TileType::FLOOR_DECORATION:
								{
									return Decoration::spawn_floor(registry, tile, position);
								}
								case config::TileType::FLOOR_HOLE:
								{
									return Floor::spawn_hole(registry, tile, position);
								}
								case config::TileType::LOW_TERRAIN:
								{
									return Terrain::spawn_low(registry, tile, position);
								}
								case config::TileType::HIGH_TERRAIN:
								{
									return Terrain::spawn_high(registry, tile, position);
								}
								default: // NOLINT(clang-diagnostic-covered-switch-default)
								{
									std::unreachable();
								}
							}
						}();

						tile_entities.emplace_back(tile_entity);
					}
				}
			}

			// doors
			{
				auto& [door_entities] = registry.emplace<room::DoorEntities>(entity);
				door_entities.reserve(room.doors.size());

				for (const auto& doors = room.doors;
				     const auto& door: doors)
				{
					// 查找瓦片集中的门瓦片
					const auto tile_it = tileset.find(door.tile_id);
					if (tile_it == tileset.end())
					{
						SPDLOG_WARN("房间[{}]引用了不存在的门瓦片:{}", room.id, door.tile_id);
						// todo: 怎么办?有没有默认纹理?
						continue;
					}

					const auto& tile = tile_it->second;

					const auto half_width = static_cast<float>(room.width) * tile_size * 0.5f;
					const auto half_height = static_cast<float>(room.height) * tile_size * 0.5f;
					const auto center_x = offset.x + half_width;
					const auto center_y = offset.y + half_height;

					const auto position = [&] noexcept -> sf::Vector2f
					{
						switch (door.direction)
						{
							case config::DoorDirection::UP:
							{
								return {center_x, offset.y + tile_size * 0.5f};
							}
							case config::DoorDirection::DOWN:
							{
								return {center_x, offset.y + static_cast<float>(room.height - 1) * tile_size + tile_size * 0.5f};
							}
							case config::DoorDirection::LEFT:
							{
								return {offset.x + tile_size * 0.5f, center_y};
							}
							case config::DoorDirection::RIGHT:
							{
								return {offset.y + static_cast<float>(room.width - 1) * tile_size + tile_size * 0.5f, center_y};
							}
							default: // NOLINT(clang-diagnostic-covered-switch-default)
							{
								std::unreachable();
							}
						}
					}();

					const auto door_entity = Door::spawn_locked(
						registry,
						tile,
						door.direction,
						door.target_room_id,
						position
					);

					if (door.requires_key)
					{
						// todo: 钥匙如何实现?
					}
					Door::set_key(registry, door_entity, entt::null);

					door_entities.emplace_back(door_entity);
				}
			}

			// enemies
			{
				auto& [enemy_entities] = registry.emplace<room::EnemyEntities>(entity);

				// todo: 生成敌人
			}
		}

		SPDLOG_INFO("生成房间[{}]成功!", room.id);

		return entity;
	}

	auto Room::destroy(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		using namespace components;

		// 销毁所有实体的物理体
		const auto destroy_entity_physics = [&](const entt::entity e) noexcept -> void
		{
			if (not registry.valid(e))
			{
				return;
			}

			// 有的tile实体没有物理体,所以要先判断一下
			if (const auto* body_id = registry.try_get<physics_body::BodyId>(e))
			{
				b2DestroyBody(body_id->id);
			}

			registry.destroy(e);
		};

		auto& [tile_entities] = registry.get<room::TileEntities>(entity);
		auto& [door_entities] = registry.get<room::DoorEntities>(entity);

		std::ranges::for_each(tile_entities, destroy_entity_physics);
		std::ranges::for_each(door_entities, destroy_entity_physics);

		tile_entities.clear();
		door_entities.clear();

		// todo: EnemyEntities需不需要处理?

		const auto& [id] = registry.get<const room::Id>(entity);
		SPDLOG_INFO("销毁房间[{}]完成", id);

		registry.destroy(entity);
	}

	auto Room::id(entt::registry& registry, const entt::entity entity) noexcept -> std::string_view
	{
		using namespace components;

		return registry.get<const room::Id>(entity).room_id;
	}

	auto Room::update_enemy_entities(entt::registry& registry, const entt::entity entity) noexcept -> bool
	{
		using namespace components;

		auto& [enemy_entities] = registry.get<room::EnemyEntities>(entity);
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

	auto Room::unlock_doors(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		using namespace components;

		auto& [door_entities] = registry.get<room::DoorEntities>(entity);

		std::ranges::for_each(
			door_entities,
			[&registry](const entt::entity door_entity) noexcept -> void
			{
				if (registry.valid(door_entity))
				{
					Door::unlock(registry, door_entity);
				}
			}
		);
	}
}
