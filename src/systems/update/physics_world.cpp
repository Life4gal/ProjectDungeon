// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/update/physics_world.hpp>

#include <components/physics_world.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::systems::update
{
	auto physics_world(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		using namespace components;

		const float time_step = delta.asSeconds();

		const auto world_id = registry.ctx().get<physics_world::Id>().id;

		constexpr int sub_step_count = 4;
		b2World_Step(world_id, time_step, sub_step_count);
	}
}
