// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/door_sensor.hpp>

#include <component/door_sensor.hpp>

#include <factory/detail/transform.hpp>
#include <factory/detail/collision.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	namespace
	{
		constexpr blueprint::CollisionBodyDef BodyDef
		{
				.type = blueprint::CollisionBodyType::STATIC,
				.fixed_rotation = true,
				.is_bullet = false,
		};
		constexpr blueprint::CollisionShapeDef ShapeDef
		{
				.material = {.friction = 0.3f, .restitution = 0},
				.density = 0,
				.category = blueprint::CollisionCategory::DOOR,
				.mask = blueprint::CollisionMask::DOOR_SENSOR,
				.is_sensor = true,
				.enable_sensor_events = true,
				.enable_contact_events = false,
		};
	}

	auto DoorSensor::spawn(entt::registry& registry, const blueprint::Room::DoorSensor& door_sensor, const blueprint::Direction direction) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// transform
		detail::attach(registry, entity, door_sensor.position);
		// collision & ShapeId
		{
			const auto body_id = detail::create_attach(registry, entity, BodyDef, door_sensor.position);

			const auto shape_id = detail::create(body_id, ShapeDef, door_sensor.shape);
			registry.emplace<door_sensor::ShapeId>(entity, shape_id);
		}
		// direction
		registry.emplace<door_sensor::Direction>(entity, direction);
		// tags
		registry.emplace<tags::DoorSensor>(entity);

		return entity;
	}

	auto DoorSensor::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::DoorSensor>();
		registry.destroy(view.begin(), view.end());
	}
}
