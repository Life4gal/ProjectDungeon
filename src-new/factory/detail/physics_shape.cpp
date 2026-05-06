// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/physics_shape.hpp>

#include <utility/physics.hpp>

#include <box2d/box2d.h>

namespace pd::factory::detail
{
	namespace
	{
		auto make_shape_def(const blueprint::PhysicsShapeDef& def) noexcept -> b2ShapeDef
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
	}

	auto create(const b2BodyId body_id, const blueprint::PhysicsShapeCircle& circle, const blueprint::Scale scale) noexcept -> b2ShapeId
	{
		using utility::Physics;

		const auto radius = Physics::to_physics(circle.radius * std::ranges::max(scale.x, scale.y));

		const auto def = make_shape_def(circle.def);
		const b2Circle shape{.center = {.x = 0, .y = 0}, .radius = radius};

		return b2CreateCircleShape(body_id, &def, &shape);
	}

	auto create(const b2BodyId body_id, const blueprint::PhysicsShapeCircle& circle, const blueprint::Sprite& sprite) noexcept -> b2ShapeId
	{
		return create(body_id, circle, sprite.scale);
	}

	auto create(const b2BodyId body_id, const blueprint::PhysicsShapeCircle& circle, const blueprint::SpriteAnimation& sprite_animation) noexcept -> b2ShapeId
	{
		return create(body_id, circle, sprite_animation.scale);
	}

	auto create(const b2BodyId body_id, const blueprint::PhysicsShapeCapsule& capsule, const blueprint::Scale scale) noexcept -> b2ShapeId
	{
		using utility::Physics;

		const auto center1 = Physics::to_physics({capsule.center1.x * scale.x, capsule.center1.y * scale.y});
		const auto center2 = Physics::to_physics({capsule.center2.x * scale.x, capsule.center2.y * scale.y});
		const auto radius = Physics::to_physics(capsule.radius * std::ranges::max(scale.x, scale.y));

		const auto def = make_shape_def(capsule.def);
		const b2Capsule shape{.center1 = center1, .center2 = center2, .radius = radius};

		return b2CreateCapsuleShape(body_id, &def, &shape);
	}

	auto create(const b2BodyId body_id, const blueprint::PhysicsShapeCapsule& capsule, const blueprint::Sprite& sprite) noexcept -> b2ShapeId
	{
		return create(body_id, capsule, sprite.scale);
	}

	auto create(const b2BodyId body_id, const blueprint::PhysicsShapeCapsule& capsule, const blueprint::SpriteAnimation& sprite_animation) noexcept -> b2ShapeId
	{
		return create(body_id, capsule, sprite_animation.scale);
	}

	auto create(const b2BodyId body_id, const blueprint::PhysicsShapeBox& box, const blueprint::Scale scale) noexcept -> b2ShapeId
	{
		using utility::Physics;

		const auto width = Physics::to_physics(box.size.width * scale.x);
		const auto height = Physics::to_physics(box.size.height * scale.y);

		const auto def = make_shape_def(box.def);
		const auto shape = b2MakeBox(width / 2, height / 2);

		return b2CreatePolygonShape(body_id, &def, &shape);
	}

	auto create(const b2BodyId body_id, const blueprint::PhysicsShapeBox& box, const blueprint::Sprite& sprite) noexcept -> b2ShapeId
	{
		return create(body_id, box, sprite.scale);
	}

	auto create(const b2BodyId body_id, const blueprint::PhysicsShapeBox& box, const blueprint::SpriteAnimation& sprite_animation) noexcept -> b2ShapeId
	{
		return create(body_id, box, sprite_animation.scale);
	}

	auto create(const b2BodyId body_id, const blueprint::PhysicsShapeOffsetBox& offset_box, const blueprint::Scale scale) noexcept -> b2ShapeId
	{
		using utility::Physics;

		const auto width = Physics::to_physics(offset_box.size.width * scale.x);
		const auto height = Physics::to_physics(offset_box.size.height * scale.y);
		const auto center = Physics::to_physics({offset_box.offset.x * scale.x, offset_box.offset.y * scale.y});
		const auto r = Physics::to_physics(sf::degrees(offset_box.rotation));

		const auto def = make_shape_def(offset_box.def);
		const auto shape = b2MakeOffsetBox(width / 2, height / 2, center, r);

		return b2CreatePolygonShape(body_id, &def, &shape);
	}

	auto create(const b2BodyId body_id, const blueprint::PhysicsShapeOffsetBox& offset_box, const blueprint::Sprite& sprite) noexcept -> b2ShapeId
	{
		return create(body_id, offset_box, sprite.scale);
	}

	auto create(const b2BodyId body_id, const blueprint::PhysicsShapeOffsetBox& offset_box, const blueprint::SpriteAnimation& sprite_animation) noexcept -> b2ShapeId
	{
		return create(body_id, offset_box, sprite_animation.scale);
	}
}
