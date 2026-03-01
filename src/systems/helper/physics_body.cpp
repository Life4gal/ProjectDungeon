// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/physics_body.hpp>

#include <components/physics_body.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::systems::helper
{
	auto PhysicsBody::attach(
		entt::registry& registry,
		const entt::entity entity,
		const b2WorldId world_id,
		const b2BodyDef& body_def
	) noexcept -> void
	{
		using namespace components;

		if (auto* body_id = registry.try_get<physics_body::BodyId>(entity);
			body_id != nullptr)
		{
			b2DestroyBody(body_id->id);

			body_id->id = b2CreateBody(world_id, &body_def);
		}

		const auto body_id = b2CreateBody(world_id, &body_def);
		registry.emplace<physics_body::BodyId>(entity, body_id);
	}

	auto PhysicsBody::deattach(entt::registry& registry, const entt::entity entity_with_physics_body) noexcept -> void
	{
		using namespace components;

		if (const auto* body_id = registry.try_get<physics_body::BodyId>(entity_with_physics_body);
			body_id != nullptr)
		{
			b2DestroyBody(body_id->id);

			registry.erase<physics_body::BodyId>(entity_with_physics_body);
		}
	}

	auto PhysicsBody::get_position(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> b2Vec2
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<const physics_body::BodyId>(entity_with_physics_body);

		return b2Body_GetPosition(body_id);
	}

	auto PhysicsBody::set_position(entt::registry& registry, const entt::entity entity_with_physics_body, const b2Vec2 position) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity_with_physics_body));

		const auto rotation = get_rotation(registry, entity_with_physics_body);

		set_transform(registry, entity_with_physics_body, {.p = position, .q = rotation});
	}

	auto PhysicsBody::get_rotation(entt::registry& registry, const entt::entity entity_with_physics_body) noexcept -> b2Rot
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<const physics_body::BodyId>(entity_with_physics_body);

		return b2Body_GetRotation(body_id);
	}

	auto PhysicsBody::set_rotation(entt::registry& registry, const entt::entity entity_with_physics_body, const b2Rot rotation) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity_with_physics_body));

		const auto position = get_position(registry, entity_with_physics_body);

		set_transform(registry, entity_with_physics_body, {.p = position, .q = rotation});
	}

	auto PhysicsBody::get_transform(entt::registry& registry, const entt::entity entity_with_physics_body) noexcept -> b2Transform
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<const physics_body::BodyId>(entity_with_physics_body);

		return b2Body_GetTransform(body_id);
	}

	auto PhysicsBody::set_transform(entt::registry& registry, const entt::entity entity_with_physics_body, const b2Transform transform) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<const physics_body::BodyId>(entity_with_physics_body);

		b2Body_SetTransform(body_id, transform.p, transform.q);
	}

	auto PhysicsBody::attach_shape(entt::registry& registry, const entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Circle& circle) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<const physics_body::BodyId>(entity_with_physics_body);

		b2CreateCircleShape(body_id, &shape_def, &circle);
	}

	auto PhysicsBody::attach_shape(entt::registry& registry, const entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Capsule& capsule) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<const physics_body::BodyId>(entity_with_physics_body);

		b2CreateCapsuleShape(body_id, &shape_def, &capsule);
	}

	auto PhysicsBody::attach_shape(entt::registry& registry, const entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Polygon& polygon) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<const physics_body::BodyId>(entity_with_physics_body);

		b2CreatePolygonShape(body_id, &shape_def, &polygon);
	}

	auto PhysicsBody::deattach_shape(entt::registry& registry, const entt::entity entity_with_physics_body) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<const physics_body::BodyId>(entity_with_physics_body);

		if (const auto count = b2Body_GetShapeCount(body_id);
			count != 0)
		{
			const auto shape_ids = std::make_unique<b2ShapeId[]>(static_cast<std::size_t>(count));
			b2Body_GetShapes(body_id, shape_ids.get(), count);

			for (const auto shape_id: std::views::counted(shape_ids.get(), count))
			{
				b2DestroyShape(shape_id, false);
			}
		}
	}

	auto PhysicsBody::apply_force_to_center(entt::registry& registry, const entt::entity entity_with_physics_body, const sf::Vector2f factor) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<const physics_body::BodyId>(entity_with_physics_body);

		const auto mass = b2Body_GetMass(body_id);
		const auto force = mass * factor;

		b2Body_ApplyForceToCenter(body_id, b2Vec2{force.x, force.y}, true);
	}
}
