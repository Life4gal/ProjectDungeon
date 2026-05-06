// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/door.hpp>

#include <component/door.hpp>

#include <factory/detail/position.hpp>
#include <factory/detail/sprite.hpp>
#include <factory/detail/physics_body.hpp>
#include <factory/detail/physics_shape.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	auto Door::spawn(entt::registry& registry, const blueprint::Door& door) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// position
		detail::attach(registry, entity, door.position);
		// sprite
		detail::attach(registry, entity, door.sprite);
		// physics_body & physics_shape
		{
			const auto body_id = detail::create_attach(registry, entity, door.physics_body, door.position);

			const auto door_shape_id = detail::create(body_id, door.physics_shape_door, door.sprite);
			registry.emplace<door::PhysicsShapeDoor>(entity, door_shape_id);

			const auto sensor_shape_id = detail::create(body_id, door.physics_shape_sensor, door.sprite);
			registry.emplace<door::PhysicsShapeSensor>(entity, sensor_shape_id);

			const auto blocker_shape_id = detail::create(body_id, door.physics_shape_blocker, door.sprite);
			registry.emplace<door::PhysicsShapeBlocker>(entity, blocker_shape_id);
		}

		// direction
		registry.emplace<door::Direction>(entity, static_cast<door::Direction>(door.direction));

		registry.emplace<tags::Door>(entity);

		return entity;
	}

	auto Door::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::Door>();
		registry.destroy(view.begin(), view.end());
	}
}
