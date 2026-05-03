// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/physics_body.hpp>

#include <utility/physics.hpp>

#include <component/physics_body.hpp>

#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <box2d/box2d.h>
#include <entt/entity/registry.hpp>

namespace pd::factory::detail
{
	auto create_attach(entt::registry& registry, const entt::entity entity, const blueprint::Transform& transform, const blueprint::PhysicsBody& physics_body) noexcept -> b2BodyId
	{
		using namespace component;

		auto def = b2DefaultBodyDef();
		def.type = static_cast<b2BodyType>(physics_body.type);
		def.position = utility::Physics::to_physics({transform.x, transform.y});
		def.rotation = utility::Physics::to_physics(sf::degrees(transform.rotation));
		def.userData = utility::Physics::to_user_data(entity);
		def.fixedRotation = physics_body.fixed_rotation;
		def.isBullet = physics_body.is_bullet;

		auto body_id = b2CreateBody(utility::Physics::world_id, &def);

		registry.emplace<physics_body::Id>(entity, body_id);

		return body_id;
	}
}
