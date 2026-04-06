// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <attacher/physics_body.hpp>

#include <bit>

#include <components/physics_body.hpp>

// #include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::attacher
{
	using namespace components;

	auto PhysicsBody::ShapeAttacher::attach(const b2ShapeDef& shape_def, const b2Circle& circle) const noexcept -> b2ShapeId
	{
		return b2CreateCircleShape(body_id_, &shape_def, &circle);
	}

	auto PhysicsBody::ShapeAttacher::attach(const b2ShapeDef& shape_def, const b2Capsule& capsule) const noexcept -> b2ShapeId
	{
		return b2CreateCapsuleShape(body_id_, &shape_def, &capsule);
	}

	auto PhysicsBody::ShapeAttacher::attach(const b2ShapeDef& shape_def, const b2Polygon& polygon) const noexcept -> b2ShapeId
	{
		return b2CreatePolygonShape(body_id_, &shape_def, &polygon);
	}

	auto PhysicsBody::attach(entt::registry& registry, const entt::entity entity, const b2WorldId world_id, const b2BodyDef& body_def) noexcept -> ShapeAttacher
	{
		const auto body_id = b2CreateBody(world_id, &body_def);

		if (auto* id = registry.try_get<physics_body::BodyId>(entity); id == nullptr)
		{
			registry.emplace<physics_body::BodyId>(entity, body_id);
		}
		else
		{
			b2DestroyBody(id->id);
			id->id = body_id;
		}

		return ShapeAttacher{body_id};
	}

	auto PhysicsBody::deattach(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity));
		//
		// 	const auto [body_id] = registry.get<const physics_body::BodyId>(entity);
		// 	b2DestroyBody(body_id);

		const auto* body_id = registry.try_get<const physics_body::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}
		b2DestroyBody(body_id->id);

		registry.remove<physics_body::BodyId>(entity);
	}

	auto PhysicsBody::disable(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity));
		//
		// 	const auto [body_id] = registry.get<const physics_body::BodyId>(entity);
		// 	b2Body_Disable(body_id);

		const auto* body_id = registry.try_get<const physics_body::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		b2Body_Disable(body_id->id);
	}

	auto PhysicsBody::enable(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity));
		//
		// 	const auto [body_id] = registry.get<const physics_body::BodyId>(entity);
		// 	b2Body_Enable(body_id);

		const auto* body_id = registry.try_get<const physics_body::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		b2Body_Enable(body_id->id);
	}

	auto PhysicsBody::get_position(entt::registry& registry, const entt::entity entity) noexcept -> b2Vec2
	{
		// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity));
		//
		// 	const auto [body_id] = registry.get<const physics_body::BodyId>(entity);
		// 	return b2Body_GetPosition(body_id);

		const auto* body_id = registry.try_get<const physics_body::BodyId>(entity);
		if (body_id == nullptr)
		{
			return b2Vec2_zero;
		}

		return b2Body_GetPosition(body_id->id);
	}

	auto PhysicsBody::set_position(entt::registry& registry, const entt::entity entity, const b2Vec2 new_position) noexcept -> void
	{
		// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity));
		//
		// 	const auto [body_id] = registry.get<const physics_body::BodyId>(entity);
		// 	const auto rotation = b2Body_GetRotation(body_id);
		// 	b2Body_SetTransform(body_id, new_position, rotation);

		const auto* body_id = registry.try_get<const physics_body::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		const auto rotation = b2Body_GetRotation(body_id->id);
		b2Body_SetTransform(body_id->id, new_position, rotation);
	}

	auto PhysicsBody::get_rotation(entt::registry& registry, const entt::entity entity) noexcept -> b2Rot
	{
		// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity));
		//
		// 	const auto [body_id] = registry.get<const physics_body::BodyId>(entity);
		// 	return b2Body_GetRotation(body_id);

		const auto* body_id = registry.try_get<const physics_body::BodyId>(entity);
		if (body_id == nullptr)
		{
			return b2Rot_identity;
		}

		return b2Body_GetRotation(body_id->id);
	}

	auto PhysicsBody::set_rotation(entt::registry& registry, const entt::entity entity, const b2Rot new_rotation) noexcept -> void
	{
		// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity));
		//
		// 	const auto [body_id] = registry.get<const physics_body::BodyId>(entity);
		// 	const auto position = b2Body_GetPosition(body_id);
		// 	b2Body_SetTransform(body_id, position, new_rotation);

		const auto* body_id = registry.try_get<const physics_body::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		const auto position = b2Body_GetPosition(body_id->id);
		b2Body_SetTransform(body_id->id, position, new_rotation);
	}

	auto PhysicsBody::get_transform(entt::registry& registry, const entt::entity entity) noexcept -> b2Transform
	{
		// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity));
		//
		// 	const auto [body_id] = registry.get<const physics_body::BodyId>(entity);
		// 	return b2Body_GetTransform(body_id);

		const auto* body_id = registry.try_get<const physics_body::BodyId>(entity);
		if (body_id == nullptr)
		{
			return b2Transform_identity;
		}

		return b2Body_GetTransform(body_id->id);
	}

	auto PhysicsBody::set_transform(entt::registry& registry, const entt::entity entity, const b2Transform new_transform) noexcept -> void
	{
		// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_body::BodyId>(entity));
		//
		// 	const auto [body_id] = registry.get<const physics_body::BodyId>(entity);
		// 	b2Body_SetTransform(body_id, transform.p, transform.q);

		const auto* body_id = registry.try_get<const physics_body::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		b2Body_SetTransform(body_id->id, new_transform.p, new_transform.q);
	}

	namespace
	{
		constexpr auto PixelsPerMeter = 32.f;
		constexpr auto MetersPerPixel = 1.f / PixelsPerMeter;

		[[nodiscard]] /*constexpr*/ auto from_physics(const b2Vec2 physics) noexcept -> sf::Vector2f
		{
			const auto p = PixelsPerMeter * physics;
			return {p.x, p.y};
		}

		[[nodiscard]] /*constexpr*/ auto to_physics(const sf::Vector2f pixels) noexcept -> b2Vec2
		{
			const auto p = MetersPerPixel * pixels;
			return {p.x, p.y};
		}
	}

	auto PhysicsBody::to_user_data(const entt::entity entity) noexcept -> void*
	{
		return std::bit_cast<void*>(static_cast<std::uintptr_t>(entt::to_integral(entity)));
	}

	auto PhysicsBody::to_entity(const void* user_data) noexcept -> entt::entity
	{
		return entt::entity{static_cast<std::underlying_type_t<entt::entity>>(std::bit_cast<std::uintptr_t>(user_data))};
	}

	auto PhysicsBody::physics_position_of(const sf::Vector2f position) noexcept -> b2Vec2
	{
		return to_physics(position);
	}

	auto PhysicsBody::position_of(const b2Vec2 physics_position) noexcept -> sf::Vector2f
	{
		return from_physics(physics_position);
	}

	auto PhysicsBody::physics_size_of(const sf::Vector2f size) noexcept -> b2Vec2
	{
		return to_physics(size);
	}

	auto PhysicsBody::size_of(const b2Vec2 physics_size) noexcept -> sf::Vector2f
	{
		return from_physics(physics_size);
	}

	auto PhysicsBody::physics_rotation_of(const sf::Angle angle) noexcept -> b2Rot
	{
		return b2MakeRot(angle.asRadians());
	}

	auto PhysicsBody::rotation_of(const b2Rot physics_angle) noexcept -> sf::Angle
	{
		const auto angle = b2Rot_GetAngle(physics_angle);

		return sf::radians(angle);
	}
}
