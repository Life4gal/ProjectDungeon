// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/room.hpp>

#include <blueprint/def_name.hpp>

#include <component/room.hpp>
#include <component/enemy.hpp>

#include <factory/door.hpp>
#include <factory/bounding.hpp>
#include <factory/tile.hpp>
#include <factory/enemy.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::factory
{
	using namespace component;

	auto Room::spawn(entt::registry& registry, const blueprint::Room& room) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// ============================================
		// 位置 + 大小
		// ============================================

		// layout position
		registry.emplace<room::LayoutPosition>(entity, sf::Vector2u{room.layout_position.x, room.layout_position.y});
		// position
		registry.emplace<room::Position>(entity, sf::Vector2f{room.position.x, room.position.y});
		// size
		registry.emplace<room::Size>(entity, sf::Vector2f{room.size.width, room.size.height});

		// ============================================
		// 门
		// ============================================

		{
			const auto neighbors_value = std::to_underlying(room.neighbors);
			// const auto neighbors_count = std::popcount(neighbors_value);

			auto& [doors] = registry.emplace<room::Doors>(entity);
			doors.fill(entt::null);

			const auto do_create = [&](const blueprint::DirectionMask mask, const blueprint::Direction direction) noexcept -> void
			{
				if (neighbors_value & std::to_underlying(mask))
				{
					SPDLOG_INFO(
						"创建房间(0x{:08X})[{}:{}]位于({})的门",
						entt::to_integral(entity),
						room.layout_position.x,
						room.layout_position.y,
						prometheus::meta::name_of(direction)
					);

					const auto& def = room.doors[std::to_underlying(direction)];
					auto& door = doors[std::to_underlying(direction)];

					door = Door::spawn(registry, def, direction);

					// 设置门所属房间实体
					registry.emplace<door::Room>(door, entity); // NOLINT(readability-suspicious-call-argument)
					// 先将门的目标房间设置为当前房间
					// 如此在factory::Level我们便可以确定遍历到的门实体属于哪个房间
					registry.emplace<door::TargetRoom>(door, entity); // NOLINT(readability-suspicious-call-argument)
				}
			};

			do_create(blueprint::DirectionMask::NORTH, blueprint::Direction::NORTH);
			do_create(blueprint::DirectionMask::SOUTH, blueprint::Direction::SOUTH);
			do_create(blueprint::DirectionMask::WEST, blueprint::Direction::WEST);
			do_create(blueprint::DirectionMask::EAST, blueprint::Direction::EAST);
		}

		// ============================================
		// 房间边界
		// ============================================

		{
			const auto bounding_entity = Bounding::spawn(registry, room.bounding);

			// 设置房间边界所属房间
			registry.emplace<bounding::Room>(bounding_entity, entity); // NOLINT(readability-suspicious-call-argument)

			registry.emplace<room::Bounding>(entity, bounding_entity);
		}

		// ============================================
		// 瓦片
		// ============================================

		{
			auto& [tiles] = registry.emplace<room::Tiles>(entity);
			tiles.reserve(room.tiles.size());

			for (const auto& tile: room.tiles)
			{
				const auto tile_entity = Tile::spawn(registry, tile);

				tiles.emplace_back(tile_entity);
			}
		}

		// ============================================
		// 敌人
		// ============================================

		{
			auto& [enemies] = registry.emplace<room::Enemies>(entity);
			enemies.reserve(room.enemies.size());

			for (const auto& enemy: room.enemies)
			{
				const auto enemy_entity = Enemy::spawn(registry, enemy);

				// 设置敌人所属房间
				registry.emplace<enemy::Room>(enemy_entity, entity); // NOLINT(readability-suspicious-call-argument)

				enemies.push_back(enemy_entity);
			}
		}

		// tags
		registry.emplace<tags::Room>(entity);

		return entity;
	}

	auto Room::destroy_all(entt::registry& registry) noexcept -> void
	{
		// 门
		Door::destroy_all(registry);
		// 房间边界
		Bounding::destroy_all(registry);
		// 瓦片
		Tile::destroy_all(registry);
		// 敌人
		Enemy::destroy_all(registry);

		const auto view = registry.view<tags::Room>();
		registry.destroy(view.begin(), view.end());
	}
}
