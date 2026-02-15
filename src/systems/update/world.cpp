// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/update/world.hpp>

#include <components/world.hpp>

#include <ctx/world.hpp>

namespace pd::systems::update
{
	auto world(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		using namespace components::world;

		ctx::World::update_frame_delta(registry, delta);
		ctx::World::update_total_elapsed(registry, delta);

		if (not ctx::World::is_pause(registry))
		{
			ctx::World::update_play_elapsed(registry, delta);
		}
	}
}
