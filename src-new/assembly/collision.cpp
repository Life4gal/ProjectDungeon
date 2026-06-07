// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <assembly/collision.hpp>

#include <utility/physics.hpp>

#include <component/collision.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::assembly
{
	using namespace component;
	using utility::Physics;

	namespace
	{
		[[nodiscard]] auto make_shape_def(const blueprint::CollisionShapeDef& shape_def) noexcept -> b2ShapeDef
		{
			auto def = b2DefaultShapeDef();

			// user data
			// 这决定了我们如何派分物理事件
			def.userData = Physics::to_user_data(shape_def.category);

			// material
			def.material.friction = shape_def.material.friction;
			def.material.restitution = shape_def.material.restitution;

			// density
			def.density = shape_def.density;

			// filter
			def.filter = b2DefaultFilter();
			def.filter.categoryBits = std::to_underlying(shape_def.category);
			def.filter.maskBits = std::to_underlying(shape_def.mask);

			// sensor
			def.isSensor = shape_def.is_sensor;

			// events
			def.enableSensorEvents = shape_def.enable_sensor_events;
			def.enableContactEvents = shape_def.enable_contact_events;

			return def;
		}
	}

	auto Collision::make_body(
		entt::registry& registry,
		const entt::entity entity,
		const blueprint::CollisionBodyDef& body_def,
		const blueprint::Position position
	) noexcept -> b2BodyId
	{
		auto def = b2DefaultBodyDef();
		def.type = static_cast<b2BodyType>(body_def.type);
		def.position = Physics::to_physics({position.x, position.y});
		// 不设置刚体旋转
		def.rotation = b2MakeRot(0);
		def.userData = Physics::to_user_data(entity);
		def.fixedRotation = body_def.fixed_rotation;
		def.isBullet = body_def.is_bullet;

		auto body_id = b2CreateBody(Physics::world_id, &def);

		registry.emplace<collision::BodyId>(entity, body_id);

		return body_id;
	}

	auto Collision::make_shape(
		const b2BodyId body_id,
		const blueprint::CollisionShapeDef& shape_def,
		const blueprint::CollisionShape::Circle& circle
	) noexcept -> b2ShapeId
	{
		const auto center = Physics::to_physics({circle.center.x, circle.center.y});
		const auto radius = Physics::to_physics(circle.radius);

		const auto def = make_shape_def(shape_def);
		const b2Circle shape{.center = center, .radius = radius};

		return b2CreateCircleShape(body_id, &def, &shape);
	}

	auto Collision::make_shape(
		const b2BodyId body_id,
		const blueprint::CollisionShapeDef& shape_def,
		const blueprint::CollisionShape::Capsule& capsule
	) noexcept -> b2ShapeId
	{
		const auto center1 = Physics::to_physics({capsule.center1.x, capsule.center1.y});
		const auto center2 = Physics::to_physics({capsule.center2.x, capsule.center2.y});
		const auto radius = Physics::to_physics(capsule.radius);

		const auto def = make_shape_def(shape_def);
		const b2Capsule shape{.center1 = center1, .center2 = center2, .radius = radius};

		return b2CreateCapsuleShape(body_id, &def, &shape);
	}

	auto Collision::make_shape(
		const b2BodyId body_id,
		const blueprint::CollisionShapeDef& shape_def,
		const blueprint::CollisionShape::Box& box
	) noexcept -> b2ShapeId
	{
		const auto width = Physics::to_physics(box.size.width);
		const auto height = Physics::to_physics(box.size.height);

		const auto def = make_shape_def(shape_def);
		const auto shape = b2MakeBox(width / 2, height / 2);

		return b2CreatePolygonShape(body_id, &def, &shape);
	}

	auto Collision::make_shape(
		const b2BodyId body_id,
		const blueprint::CollisionShapeDef& shape_def,
		const blueprint::CollisionShape::OffsetBox& offset_box
	) noexcept -> b2ShapeId
	{
		const auto width = Physics::to_physics(offset_box.size.width);
		const auto height = Physics::to_physics(offset_box.size.height);
		const auto center = Physics::to_physics({offset_box.center.x, offset_box.center.y});
		const auto r = Physics::to_physics(sf::degrees(offset_box.rotation.rotation));

		const auto def = make_shape_def(shape_def);
		const auto shape = b2MakeOffsetBox(width / 2, height / 2, center, r);

		return b2CreatePolygonShape(body_id, &def, &shape);
	}

	auto Collision::make_shape(
		const b2BodyId body_id,
		const blueprint::CollisionShapeDef& shape_def,
		const blueprint::CollisionShape::Segment& segment
	) noexcept -> b2ShapeId
	{
		const auto p1 = Physics::to_physics({segment.point1.x, segment.point1.y});
		const auto p2 = Physics::to_physics({segment.point2.x, segment.point2.y});

		const auto def = make_shape_def(shape_def);
		const b2Segment shape{.point1 = p1, .point2 = p2};

		return b2CreateSegmentShape(body_id, &def, &shape);
	}

	auto Collision::make_shape(
		const b2BodyId body_id,
		const blueprint::CollisionShape& shape
	) noexcept -> b2ShapeId
	{
		return std::visit(
			[&](const auto s) noexcept -> b2ShapeId
			{
				return make_shape(body_id, shape.def, s);
			},
			shape.shape
		);
	}

	auto Collision::make(
		entt::registry& registry,
		const entt::entity entity,
		const blueprint::Collision& collision,
		const blueprint::Position position
	) noexcept -> void
	{
		const auto& [def, shapes] = collision;

		const auto body_id = make_body(registry, entity, def, position);

		auto& [shape_ids] = registry.emplace<collision::ShapeIds>(entity);
		shape_ids.reserve(shapes.size());

		for (const auto& shape: shapes)
		{
			const auto shape_id = make_shape(body_id, shape);

			shape_ids.push_back(shape_id);
		}
	}
}
