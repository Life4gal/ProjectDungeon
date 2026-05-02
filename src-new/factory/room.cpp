// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/room.hpp>

#include <factory/floor.hpp>
#include <factory/wall.hpp>
#include <factory/door.hpp>

namespace pd::factory
{
	auto Room::create(entt::registry& registry, const blueprint::Room& room) noexcept -> void
	{
		for (auto floor: room.floors)
		{
			floor.transform.x += room.offset_x;
			floor.transform.y += room.offset_y;

			Floor::spawn(registry, floor);
		}
		for (auto wall: room.walls)
		{
			wall.transform.x += room.offset_x;
			wall.transform.y += room.offset_y;

			Wall::spawn(registry, wall);
		}
		for (auto door: room.doors)
		{
			door.transform.x += room.offset_x;
			door.transform.y += room.offset_y;

			Door::spawn(registry, door);
		}
	}

	auto Room::destroy(entt::registry& registry) noexcept -> void
	{
		Floor::destroy_all(registry);
		Wall::destroy_all(registry);
		Door::destroy_all(registry);
	}
}
