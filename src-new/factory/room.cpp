// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/room.hpp>

#include <component/room.hpp>
#include <component/door.hpp>

#include <factory/floor.hpp>
#include <factory/wall.hpp>
#include <factory/door.hpp>
#include <factory/enemy.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	auto Room::spawn(entt::registry& registry, const blueprint::Room& room) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// floors
		for (const auto& floor: room.floors)
		{
			Floor::spawn(registry, floor);
		}
		// walls
		for (const auto& wall: room.walls)
		{
			Wall::spawn(registry, wall);
		}
		// doors
		for (const auto& door: room.doors)
		{
			const auto door_entity = Door::spawn(registry, door);

			// 先将门的目标房间设置为当前房间
			// 如此在factory::Level我们便可以确定遍历到的门实体属于那个房间
			registry.emplace<door::TargetRoom>(door_entity, entity); // NOLINT(readability-suspicious-call-argument)
		}
		// enemies
		{
			auto& [enemies] = registry.emplace<room::Enemies>(entity);
			enemies.reserve(room.enemies.size());

			for (const auto& enemy: room.enemies)
			{
				const auto enemy_entity = Enemy::spawn(registry, enemy);
				enemies.push_back(enemy_entity);
			}
		}

		return entity;
	}

	auto Room::destroy_all(entt::registry& registry) noexcept -> void
	{
		Floor::destroy_all(registry);
		Wall::destroy_all(registry);
		Door::destroy_all(registry);
		Enemy::destroy_all(registry);

		const auto view = registry.view<room::Room>();
		registry.destroy(view.begin(), view.end());
	}
}
