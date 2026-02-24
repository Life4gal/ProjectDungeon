// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/initialize/physics_world.hpp>

#include <systems/helper/physics_world.hpp>

namespace pd::systems::initialize
{
	auto physics_world(entt::registry& registry) noexcept -> void
	{
		using namespace systems;

		helper::PhysicsWorld::create(registry);
	}
}
