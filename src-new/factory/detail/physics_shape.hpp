// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <utility/physics.hpp>

#include <blueprint/detail/physics_shape.hpp>

#include <box2d/box2d.h>

namespace pd::factory::detail
{
	[[nodiscard]] inline auto make_shape_def(const blueprint::PhysicsShapeDef& def) noexcept -> b2ShapeDef
	{
		auto d = b2DefaultShapeDef();

		// material
		d.material.friction = def.material.friction;
		d.material.restitution = def.material.restitution;

		// sensor
		d.isSensor = def.is_sensor;

		// events
		d.enableSensorEvents = def.enable_sensor_events;
		d.enableContactEvents = def.enable_contact_events;

		return d;
	}

	[[nodiscard]] inline auto create(
		const b2BodyId body_id,
		const blueprint::PhysicsShapeCircle& circle
	) noexcept -> b2ShapeId
	{
		using utility::Physics;

		const auto def = make_shape_def(circle.def);
		const b2Circle shape{.center = {0, 0}, .radius = Physics::to_physics(circle.radius)};

		return b2CreateCircleShape(body_id, &def, &shape);
	}

	[[nodiscard]] inline auto create(
		const b2BodyId body_id,
		const blueprint::PhysicsShapeCapsule& capsule
	) noexcept -> b2ShapeId
	{
		using utility::Physics;

		const auto def = make_shape_def(capsule.def);
		const b2Capsule shape
		{
				.center1 = Physics::to_physics({capsule.center1_offset_x, capsule.center1_offset_y}),
				.center2 = Physics::to_physics({capsule.center2_offset_x, capsule.center2_offset_y}),
				.radius = Physics::to_physics(capsule.radius)
		};

		return b2CreateCapsuleShape(body_id, &def, &shape);
	}

	[[nodiscard]] inline auto create(
		const b2BodyId body_id,
		const blueprint::PhysicsShapeBox& box
	) noexcept -> b2ShapeId
	{
		using utility::Physics;

		const auto def = make_shape_def(box.def);
		const auto shape = b2MakeBox(Physics::to_physics(box.width / 2), Physics::to_physics(box.height / 2));

		return b2CreatePolygonShape(body_id, &def, &shape);
	}

	[[nodiscard]] inline auto create(
		const b2BodyId body_id,
		const blueprint::PhysicsShapeOffsetBox& offset_box
	) noexcept -> b2ShapeId
	{
		using utility::Physics;

		const auto def = make_shape_def(offset_box.def);
		const auto shape = b2MakeOffsetBox(
			Physics::to_physics(offset_box.width / 2),
			Physics::to_physics(offset_box.height / 2),
			Physics::to_physics({offset_box.offset_x, offset_box.offset_y}),
			Physics::to_physics(sf::degrees(offset_box.rotation))
		);

		return b2CreatePolygonShape(body_id, &def, &shape);
	}
}
