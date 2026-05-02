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

		// user data
		// 这决定了我们如何派分物理事件
		d.userData = utility::Physics::to_user_data(def.category);

		// material
		d.material.friction = def.material.friction;
		d.material.restitution = def.material.restitution;

		// density
		d.density = def.density;

		// filter
		d.filter = b2DefaultFilter();
		d.filter.categoryBits = std::to_underlying(def.category);
		d.filter.maskBits = def.category_mask;

		// sensor
		d.isSensor = def.is_sensor;

		// events
		d.enableSensorEvents = def.enable_sensor_events;
		d.enableContactEvents = def.enable_contact_events;

		return d;
	}

	[[nodiscard]] inline auto create(
		const b2BodyId body_id,
		const blueprint::Transform& transform,
		const blueprint::PhysicsShapeCircle& circle
	) noexcept -> b2ShapeId
	{
		using utility::Physics;

		const auto radius = Physics::to_physics(circle.radius * std::ranges::max(transform.scale_x, transform.scale_y));

		const auto def = make_shape_def(circle.def);
		const b2Circle shape{.center = {0, 0}, .radius = radius};

		return b2CreateCircleShape(body_id, &def, &shape);
	}

	[[nodiscard]] inline auto create(
		const b2BodyId body_id,
		const blueprint::Transform& transform,
		const blueprint::PhysicsShapeCapsule& capsule
	) noexcept -> b2ShapeId
	{
		using utility::Physics;

		const auto center1 = Physics::to_physics({capsule.center1_offset_x * transform.scale_x, capsule.center1_offset_y * transform.scale_y});
		const auto center2 = Physics::to_physics({capsule.center2_offset_x * transform.scale_x, capsule.center2_offset_y * transform.scale_y});
		const auto radius = Physics::to_physics(capsule.radius * std::ranges::max(transform.scale_x, transform.scale_y));

		const auto def = make_shape_def(capsule.def);
		const b2Capsule shape{.center1 = center1, .center2 = center2, .radius = radius};

		return b2CreateCapsuleShape(body_id, &def, &shape);
	}

	[[nodiscard]] inline auto create(
		const b2BodyId body_id,
		const blueprint::Transform& transform,
		const blueprint::PhysicsShapeBox& box
	) noexcept -> b2ShapeId
	{
		using utility::Physics;

		const auto width = Physics::to_physics(box.width * transform.scale_x);
		const auto height = Physics::to_physics(box.height * transform.scale_y);

		const auto def = make_shape_def(box.def);
		const auto shape = b2MakeBox(width / 2, height / 2);

		return b2CreatePolygonShape(body_id, &def, &shape);
	}

	[[nodiscard]] inline auto create(
		const b2BodyId body_id,
		const blueprint::Transform& transform,
		const blueprint::PhysicsShapeOffsetBox& offset_box
	) noexcept -> b2ShapeId
	{
		using utility::Physics;

		const auto width = Physics::to_physics(offset_box.width * transform.scale_x);
		const auto height = Physics::to_physics(offset_box.height * transform.scale_y);
		const auto center = Physics::to_physics({offset_box.offset_x * transform.scale_x, offset_box.offset_y * transform.scale_y});
		const auto rotation = Physics::to_physics(sf::degrees(transform.rotation));

		const auto def = make_shape_def(offset_box.def);
		const auto shape = b2MakeOffsetBox(width / 2, height / 2, center, rotation);

		return b2CreatePolygonShape(body_id, &def, &shape);
	}
}
