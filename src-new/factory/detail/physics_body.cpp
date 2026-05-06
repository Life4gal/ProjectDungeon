// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/physics_body.hpp>

#include <utility/physics.hpp>

#include <component/physics_body.hpp>

#include <entt/entt.hpp>
#include <SFML/System.hpp>
#include <box2d/box2d.h>

namespace pd::factory::detail
{
	auto create_attach(entt::registry& registry, const entt::entity entity, const blueprint::PhysicsBody& physics_body, const blueprint::Position position) noexcept -> b2BodyId
	{
		using namespace component;

		auto def = b2DefaultBodyDef();
		def.type = static_cast<b2BodyType>(physics_body.type);
		def.position = utility::Physics::to_physics({position.x, position.y});
		// 不设置刚体旋转
		def.rotation = b2MakeRot(0);
		def.userData = utility::Physics::to_user_data(entity);
		def.fixedRotation = physics_body.fixed_rotation;
		def.isBullet = physics_body.is_bullet;

		auto body_id = b2CreateBody(utility::Physics::world_id, &def);

		registry.emplace<physics_body::Id>(entity, body_id);

		return body_id;
	}
}
