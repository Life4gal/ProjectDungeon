// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/physics_world.hpp>

#include <utility/physics.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::update
{
	auto physics_world([[maybe_unused]] entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		const auto& world_id = utility::Physics::world_id;
		const auto time_step = delta.asSeconds();
		constexpr auto sub_step_count = 4;

		b2World_Step(world_id, time_step, sub_step_count);
	}
}
