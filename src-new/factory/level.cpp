// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/level.hpp>

#include <factory/room.hpp>

namespace pd::factory
{
	auto Level::create(entt::registry& registry, const blueprint::Level& level) noexcept -> void
	{
		for (const auto& [type, room]: level.rooms)
		{
			std::ignore = type;
			Room::create(registry, room);
		}
	}

	auto Level::destroy(entt::registry& registry) noexcept -> void
	{
		Room::destroy(registry);
	}
}
