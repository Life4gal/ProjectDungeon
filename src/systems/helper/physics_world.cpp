// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/physics_world.hpp>

#include <components/physics_world.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::systems::helper
{
	auto PhysicsWorld::create(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		auto def = b2DefaultWorldDef();
		// 无重力世界
		def.gravity = b2Vec2_zero;
		const auto id = b2CreateWorld(&def);

		registry.ctx().emplace<physics_world::Id>(id);
	}

	auto PhysicsWorld::destroy(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		const auto [id] = registry.ctx().get<physics_world::Id>();
		b2DestroyWorld(id);
	}

	auto PhysicsWorld::id(entt::registry& registry) noexcept -> b2WorldId
	{
		using namespace components;

		return registry.ctx().get<physics_world::Id>().id;
	}
}
