// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/room.hpp>

#include <factory/floor.hpp>
#include <factory/wall.hpp>

namespace pd::factory
{
	auto Room::create(entt::registry& registry, const blueprint::Room& room) noexcept -> void
	{
		for (const auto& floor: room.floors)
		{
			Floor::spawn(registry, floor);
		}
		for (const auto& wall: room.walls)
		{
			Wall::spawn(registry, wall);
		}
	}

	auto Room::destroy(entt::registry& registry) noexcept -> void
	{
		Floor::destroy_all(registry);
		Wall::destroy_all(registry);
	}
}
