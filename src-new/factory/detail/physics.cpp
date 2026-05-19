// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/physics.hpp>

#include <utility/physics.hpp>

#include <component/physics.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::factory::detail
{
	using namespace component;
	using utility::Physics;

	namespace
	{
		auto make_shape_def(const blueprint::ShapeDesc& shape_desc) noexcept -> b2ShapeDef
		{
			auto d = b2DefaultShapeDef();

			// user data
			// 这决定了我们如何派分物理事件
			d.userData = Physics::to_user_data(shape_desc.category);

			// material
			d.material.friction = shape_desc.material.friction;
			d.material.restitution = shape_desc.material.restitution;

			// density
			d.density = shape_desc.density;

			// filter
			d.filter = b2DefaultFilter();
			d.filter.categoryBits = std::to_underlying(shape_desc.category);
			d.filter.maskBits = shape_desc.category_mask;

			// sensor
			d.isSensor = shape_desc.is_sensor;

			// events
			d.enableSensorEvents = shape_desc.enable_sensor_events;
			d.enableContactEvents = shape_desc.enable_contact_events;

			return d;
		}
	}

	auto create_attach(
		entt::registry& registry,
		const entt::entity entity,
		const blueprint::BodyDesc& body_desc,
		const blueprint::Position position
	) noexcept -> b2BodyId
	{
		auto def = b2DefaultBodyDef();
		def.type = static_cast<b2BodyType>(body_desc.type);
		def.position = Physics::to_physics({position.x, position.y});
		// 不设置刚体旋转
		def.rotation = b2MakeRot(0);
		def.userData = Physics::to_user_data(entity);
		def.fixedRotation = body_desc.fixed_rotation;
		def.isBullet = body_desc.is_bullet;

		auto body_id = b2CreateBody(Physics::world_id, &def);

		registry.emplace<physics::BodyId>(entity, body_id);

		return body_id;
	}

	auto create(
		const b2BodyId body_id,
		const blueprint::ShapeDesc& shape_desc,
		const blueprint::ShapeCategory::Circle& circle
	) noexcept -> b2ShapeId
	{
		const auto radius = Physics::to_physics(circle.radius);

		const auto def = make_shape_def(shape_desc);
		const b2Circle shape{.center = {.x = 0, .y = 0}, .radius = radius};

		return b2CreateCircleShape(body_id, &def, &shape);
	}

	auto create(
		const b2BodyId body_id,
		const blueprint::ShapeDesc& shape_desc,
		const blueprint::ShapeCategory::Capsule& capsule
	) noexcept -> b2ShapeId
	{
		const auto center1 = Physics::to_physics({capsule.center1.x, capsule.center1.y});
		const auto center2 = Physics::to_physics({capsule.center2.x, capsule.center2.y});
		const auto radius = Physics::to_physics(capsule.radius);

		const auto def = make_shape_def(shape_desc);
		const b2Capsule shape{.center1 = center1, .center2 = center2, .radius = radius};

		return b2CreateCapsuleShape(body_id, &def, &shape);
	}

	auto create(
		const b2BodyId body_id,
		const blueprint::ShapeDesc& shape_desc,
		const blueprint::ShapeCategory::Box& box
	) noexcept -> b2ShapeId
	{
		const auto width = Physics::to_physics(box.size.width);
		const auto height = Physics::to_physics(box.size.height);

		const auto def = make_shape_def(shape_desc);
		const auto shape = b2MakeBox(width / 2, height / 2);

		return b2CreatePolygonShape(body_id, &def, &shape);
	}

	auto create(
		const b2BodyId body_id,
		const blueprint::ShapeDesc& shape_desc,
		const blueprint::ShapeCategory::OffsetBox& offset_box
	) noexcept -> b2ShapeId
	{
		const auto width = Physics::to_physics(offset_box.size.width);
		const auto height = Physics::to_physics(offset_box.size.height);
		const auto center = Physics::to_physics({offset_box.offset.x, offset_box.offset.y});
		const auto r = Physics::to_physics(sf::degrees(offset_box.rotation.rotation));

		const auto def = make_shape_def(shape_desc);
		const auto shape = b2MakeOffsetBox(width / 2, height / 2, center, r);

		return b2CreatePolygonShape(body_id, &def, &shape);
	}
}
