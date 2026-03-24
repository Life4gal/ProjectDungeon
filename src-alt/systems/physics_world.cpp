// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/physics_world.hpp>

#include <bit>

#include <components/physics_world.hpp>

#include <prometheus/platform/os.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::systems
{
	namespace
	{
		constexpr auto pixels_per_meter = 32.f;
		constexpr auto meters_per_pixel = 1.f / pixels_per_meter;

		[[nodiscard]] /*constexpr*/ auto from_physics(const b2Vec2 physics) noexcept -> sf::Vector2f
		{
			const auto p = pixels_per_meter * physics;
			return {p.x, p.y};
		}

		[[nodiscard]] /*constexpr*/ auto to_physics(const sf::Vector2f pixels) noexcept -> b2Vec2
		{
			const auto p = meters_per_pixel * pixels;
			return {p.x, p.y};
		}
	}

	auto PhysicsWorld::create(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		auto def = b2DefaultWorldDef();
		// 无重力世界(俯视角)
		def.gravity = b2Vec2_zero;
		const auto id = b2CreateWorld(&def);

		registry.ctx().emplace<physics_world::WorldId>(id);
	}

	auto PhysicsWorld::destroy(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		const auto [id] = registry.ctx().get<physics_world::WorldId>();
		b2DestroyWorld(id);

		registry.ctx().erase<physics_world::WorldId>();
	}

	auto PhysicsWorld::id(entt::registry& registry) noexcept -> b2WorldId
	{
		using namespace components;

		const auto [id] = registry.ctx().get<const physics_world::WorldId>();

		return id;
	}

	auto PhysicsWorld::to_user_data(const entt::entity entity) noexcept -> void*
	{
		return std::bit_cast<void*>(static_cast<std::uintptr_t>(entt::to_integral(entity)));
	}

	auto PhysicsWorld::to_entity(const void* user_data) noexcept -> entt::entity
	{
		return entt::entity{static_cast<std::underlying_type_t<entt::entity>>(std::bit_cast<std::uintptr_t>(user_data))};
	}

	auto PhysicsWorld::physics_position_of(const sf::Vector2f position) noexcept -> b2Vec2
	{
		return to_physics(position);
	}

	auto PhysicsWorld::position_of(const b2Vec2 physics_position) noexcept -> sf::Vector2f
	{
		return from_physics(physics_position);
	}

	auto PhysicsWorld::physics_size_of(const sf::Vector2f size) noexcept -> b2Vec2
	{
		return to_physics(size);
	}

	auto PhysicsWorld::size_of(const b2Vec2 physics_size) noexcept -> sf::Vector2f
	{
		return from_physics(physics_size);
	}

	auto PhysicsWorld::physics_rotation_of(const sf::Angle angle) noexcept -> b2Rot
	{
		return b2MakeRot(angle.asRadians());
	}

	auto PhysicsWorld::rotation_of(const b2Rot physics_angle) noexcept -> sf::Angle
	{
		const auto angle = b2Rot_GetAngle(physics_angle);

		return sf::radians(angle);
	}

	auto PhysicsWorld::attach(entt::registry& registry, const entt::entity entity, const b2WorldId world_id, const b2BodyDef& body_def) noexcept -> void
	{
		using namespace components;

		// 如果已有物理体则先销毁
		if (registry.all_of<physics_world::BodyId>(entity))
		{
			deattach(registry, entity);
		}

		const auto body_id = b2CreateBody(world_id, &body_def);
		registry.emplace_or_replace<physics_world::BodyId>(entity, body_id);
	}

	auto PhysicsWorld::deattach(entt::registry& registry, const entt::entity entity_with_physics_body) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<physics_world::BodyId>(entity_with_physics_body);
		b2DestroyBody(body_id);

		registry.remove<physics_world::BodyId>(entity_with_physics_body);
	}

	auto PhysicsWorld::disable(entt::registry& registry, const entt::entity entity_with_physics_body) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<physics_world::BodyId>(entity_with_physics_body);
		b2Body_Disable(body_id);
	}

	auto PhysicsWorld::enable(entt::registry& registry, const entt::entity entity_with_physics_body) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<physics_world::BodyId>(entity_with_physics_body);
		b2Body_Enable(body_id);
	}

	auto PhysicsWorld::get_position(entt::registry& registry, const entt::entity entity_with_physics_body) noexcept -> b2Vec2
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<physics_world::BodyId>(entity_with_physics_body);
		return b2Body_GetPosition(body_id);
	}

	auto PhysicsWorld::set_position(entt::registry& registry, const entt::entity entity_with_physics_body, const b2Vec2 position) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<physics_world::BodyId>(entity_with_physics_body);
		const auto rotation = b2Body_GetRotation(body_id);
		b2Body_SetTransform(body_id, position, rotation);
	}

	auto PhysicsWorld::get_rotation(entt::registry& registry, const entt::entity entity_with_physics_body) noexcept -> b2Rot
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<physics_world::BodyId>(entity_with_physics_body);
		return b2Body_GetRotation(body_id);
	}

	auto PhysicsWorld::set_rotation(entt::registry& registry, const entt::entity entity_with_physics_body, const b2Rot rotation) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<physics_world::BodyId>(entity_with_physics_body);
		const auto position = b2Body_GetPosition(body_id);
		b2Body_SetTransform(body_id, position, rotation);
	}

	auto PhysicsWorld::get_transform(entt::registry& registry, const entt::entity entity_with_physics_body) noexcept -> b2Transform
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<physics_world::BodyId>(entity_with_physics_body);
		return b2Body_GetTransform(body_id);
	}

	auto PhysicsWorld::set_transform(entt::registry& registry, const entt::entity entity_with_physics_body, const b2Transform transform) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<physics_world::BodyId>(entity_with_physics_body);
		b2Body_SetTransform(body_id, transform.p, transform.q);
	}

	auto PhysicsWorld::attach_shape(entt::registry& registry, const entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Circle& circle) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<physics_world::BodyId>(entity_with_physics_body);
		b2CreateCircleShape(body_id, &shape_def, &circle);
	}

	auto PhysicsWorld::attach_shape(entt::registry& registry, const entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Capsule& capsule) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<physics_world::BodyId>(entity_with_physics_body);
		b2CreateCapsuleShape(body_id, &shape_def, &capsule);
	}

	auto PhysicsWorld::attach_shape(entt::registry& registry, const entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Polygon& polygon) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(entity_with_physics_body));

		const auto [body_id] = registry.get<physics_world::BodyId>(entity_with_physics_body);
		b2CreatePolygonShape(body_id, &shape_def, &polygon);
	}
}
