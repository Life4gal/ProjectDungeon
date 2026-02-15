// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <ctx/physics_world.hpp>

#include <components/physics_world.hpp>

#include <entt/entt.hpp>

namespace pd::ctx
{
	auto PhysicsWorld::id(entt::registry& registry) noexcept -> b2WorldId
	{
		using namespace components::physics_world;

		return registry.ctx().get<Id>().id;
	}
}
