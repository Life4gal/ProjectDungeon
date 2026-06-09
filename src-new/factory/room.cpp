// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/room.hpp>

#include <blueprint/def_name.hpp>

#include <component/room.hpp>

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

		SPDLOG_INFO(
			"\n================ 正在创建[{}]房间(Entity: 0x{:08X}, LayoutPosition: ({}:{}), Position: ({:.1f}:{:.1f})) ================",
			prometheus::meta::name_of(room.type),
			entt::to_integral(entity),
			room.layout_position.x,
			room.layout_position.y,
			room.position.x,
			room.position.y
		);

		// ============================================
		// 类型
		// ============================================

		// type
		registry.emplace<room::RoomType>(entity, room.type);

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
		// 边界(墙壁)
		// ============================================

		{
			SPDLOG_INFO("开始创建房间边界...");

			const auto bounding_entity = Bounding::spawn(registry, room.bounding);

			// 设置房间边界所属房间
			registry.emplace<bounding::Room>(bounding_entity, entity); // NOLINT(readability-suspicious-call-argument)
			// 记录该实体(应该用不到?)
			registry.emplace<room::Bounding>(entity, bounding_entity);

			SPDLOG_INFO("房间边界创建完成");
		}

		// ============================================
		// 门
		// ============================================

		{
			SPDLOG_INFO("开始创建门,共有{}个邻居房间...", room.doors.size());

			auto& [doors] = registry.emplace<room::Doors>(entity);
			doors.fill(entt::null);

			for (const auto& door: room.doors)
			{
				SPDLOG_INFO(
					"正在创建位于({})的门...",
					prometheus::meta::name_of(door.direction)
				);

				auto& door_entity = doors[std::to_underlying(door.direction)];

				door_entity = Door::spawn(registry, door);
				// 设置门所属房间实体
				registry.emplace<door::Room>(door_entity, entity); // NOLINT(readability-suspicious-call-argument)
				// 先将门的目标房间设置为当前房间
				// 如此在factory::Level我们便可以确定遍历到的门实体属于哪个房间
				registry.emplace<door::TargetRoom>(door_entity, entity); // NOLINT(readability-suspicious-call-argument)
			}

			SPDLOG_INFO("门创建完成");
		}

		// ============================================
		// 瓦片
		// ============================================

		{
			SPDLOG_INFO("开始创建瓦片,共有{}个瓦片...", room.tiles.size());

			auto& [tiles] = registry.emplace<room::Tiles>(entity);
			tiles.reserve(room.tiles.size());

			for (const auto& tile: room.tiles)
			{
				const auto tile_entity = Tile::spawn(registry, tile);

				tiles.emplace_back(tile_entity);
			}

			SPDLOG_INFO("瓦片创建完成");
		}

		// ============================================
		// 敌人
		// ============================================

		if (not room.enemies.empty())
		{
			SPDLOG_INFO("开始创建敌人,共有{}个敌人...", room.enemies.size());

			auto& [enemies] = registry.emplace<room::Enemies>(entity);
			enemies.reserve(room.enemies.size());

			for (const auto& enemy: room.enemies)
			{
				const auto enemy_entity = Enemy::spawn(registry, enemy);

				// 设置敌人所属房间
				registry.emplace<enemy::Room>(enemy_entity, entity); // NOLINT(readability-suspicious-call-argument)

				enemies.push_back(enemy_entity);
			}

			SPDLOG_INFO("敌人创建完成");
		}

		// ============================================
		// NPC
		// ============================================

		if (not room.npc.empty())
		{
			SPDLOG_INFO("开始创建NPC,共有{}个NPC...", room.npc.size());

			//

			SPDLOG_INFO("NPC创建完成");
		}

		// tags
		registry.emplace<tags::Room>(entity);
		SPDLOG_INFO("\n================ 房间创建完成 ================");

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

		SPDLOG_INFO("正在销毁所有房间...");

		const auto view = registry.view<tags::Room>();
		registry.destroy(view.begin(), view.end());

		SPDLOG_INFO("已销毁{}个房间", view.size());
	}
}
