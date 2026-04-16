// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <accessor/physics.hpp>

// bit_cast
#include <bit>

#include <components/physics.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::accessor
{
	using namespace components;

	namespace
	{
		constexpr auto PixelsPerMeter = 32.f;
		constexpr auto MetersPerPixel = 1.f / PixelsPerMeter;

		[[nodiscard]] /*constexpr*/ auto from_physics(const b2Vec2 physics) noexcept -> sf::Vector2f
		{
			const auto [x, y] = PixelsPerMeter * physics;
			return {x, y};
		}

		[[nodiscard]] /*constexpr*/ auto to_physics(const sf::Vector2f pixels) noexcept -> b2Vec2
		{
			const auto p = MetersPerPixel * pixels;
			return {.x = p.x, .y = p.y};
		}

		b2WorldId g_world_id = b2_nullWorldId;

		[[nodiscard]] auto is_valid_world() noexcept -> bool
		{
			return B2_IS_NON_NULL(g_world_id);
		}
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	auto Physics::ShapeAttacher::attach(const b2ShapeDef& shape_def, const b2Circle& circle) noexcept -> b2ShapeId
	{
		return b2CreateCircleShape(body_id_, &shape_def, &circle);
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	auto Physics::ShapeAttacher::attach(const b2ShapeDef& shape_def, const b2Capsule& capsule) noexcept -> b2ShapeId
	{
		return b2CreateCapsuleShape(body_id_, &shape_def, &capsule);
	}

	// ReSharper disable once CppMemberFunctionMayBeConst
	auto Physics::ShapeAttacher::attach(const b2ShapeDef& shape_def, const b2Polygon& polygon) noexcept -> b2ShapeId
	{
		return b2CreatePolygonShape(body_id_, &shape_def, &polygon);
	}

	auto Physics::initialize_system(entt::registry& registry) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(not is_valid_world(), "重复创建物理世界!");

		auto def = b2DefaultWorldDef();
		// 无重力世界(俯视角)
		def.gravity = b2Vec2_zero;
		g_world_id = b2CreateWorld(&def);

		// 订阅组件销毁事件,以便在组件销毁时销毁物理刚体
		// 如此便不需要在销毁实体前手动调用deattach函数销毁物理刚体组件
		registry.on_destroy<physics::BodyId>().connect<
			[](entt::registry& r, const entt::entity e) noexcept -> void
			{
				const auto [body_id] = r.get<const physics::BodyId>(e);
				b2DestroyBody(body_id);
			}
		>();
	}

	auto Physics::destroy_system([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(is_valid_world(), "未创建物理世界!");

		b2DestroyWorld(g_world_id);
		g_world_id = b2_nullWorldId;
	}

	auto Physics::attach(entt::registry& registry, const entt::entity entity, const b2BodyDef& body_def) noexcept -> ShapeAttacher
	{
		PROMETHEUS_PLATFORM_ASSUME(is_valid_world(), "未创建物理世界!");

		auto* body_id = registry.try_get<physics::BodyId>(entity);
		if (body_id != nullptr)
		{
			return ShapeAttacher{body_id->id};
		}

		const auto new_body_id = b2CreateBody(g_world_id, &body_def);
		body_id->id = new_body_id;

		return ShapeAttacher{new_body_id};
	}

	auto Physics::attach_force(entt::registry& registry, const entt::entity entity, const b2BodyDef& body_def) noexcept -> ShapeAttacher
	{
		PROMETHEUS_PLATFORM_ASSUME(is_valid_world(), "未创建物理世界!");

		const auto new_body_id = b2CreateBody(g_world_id, &body_def);

		if (auto* body_id = registry.try_get<physics::BodyId>(entity);
			body_id != nullptr)
		{
			b2DestroyBody(body_id->id);
			body_id->id = new_body_id;
		}
		else
		{
			registry.emplace<physics::BodyId>(entity, new_body_id);
		}

		return ShapeAttacher{new_body_id};
	}

	auto Physics::deattach(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(is_valid_world(), "未创建物理世界!");

		// 直接移除组件即可,因为组件销毁事件已连接到销毁物理刚体的函数
		registry.remove<physics::BodyId>(entity);
	}

	auto Physics::disable(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(is_valid_world(), "未创建物理世界!");

		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		b2Body_Disable(body_id->id);
	}

	auto Physics::enable(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(is_valid_world(), "未创建物理世界!");

		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		b2Body_Enable(body_id->id);
	}

	auto Physics::get_position(entt::registry& registry, const entt::entity entity) noexcept -> b2Vec2
	{
		PROMETHEUS_PLATFORM_ASSUME(is_valid_world(), "未创建物理世界!");

		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return b2Vec2_zero;
		}

		return b2Body_GetPosition(body_id->id);
	}

	auto Physics::set_position(entt::registry& registry, const entt::entity entity, const b2Vec2 new_position) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(is_valid_world(), "未创建物理世界!");

		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		const auto rotation = b2Body_GetRotation(body_id->id);
		b2Body_SetTransform(body_id->id, new_position, rotation);
	}

	auto Physics::get_rotation(entt::registry& registry, const entt::entity entity) noexcept -> b2Rot
	{
		PROMETHEUS_PLATFORM_ASSUME(is_valid_world(), "未创建物理世界!");

		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return b2Rot_identity;
		}

		return b2Body_GetRotation(body_id->id);
	}

	auto Physics::set_rotation(entt::registry& registry, const entt::entity entity, const b2Rot new_rotation) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(is_valid_world(), "未创建物理世界!");

		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		const auto position = b2Body_GetPosition(body_id->id);
		b2Body_SetTransform(body_id->id, position, new_rotation);
	}

	auto Physics::get_transform(entt::registry& registry, const entt::entity entity) noexcept -> b2Transform
	{
		PROMETHEUS_PLATFORM_ASSUME(is_valid_world(), "未创建物理世界!");

		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return b2Transform_identity;
		}

		return b2Body_GetTransform(body_id->id);
	}

	auto Physics::set_transform(entt::registry& registry, const entt::entity entity, const b2Transform new_transform) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(is_valid_world(), "未创建物理世界!");

		const auto* body_id = registry.try_get<const physics::BodyId>(entity);
		if (body_id == nullptr)
		{
			return;
		}

		b2Body_SetTransform(body_id->id, new_transform.p, new_transform.q);
	}

	auto Physics::to_user_data(const entt::entity entity) noexcept -> void*
	{
		return std::bit_cast<void*>(static_cast<std::uintptr_t>(entt::to_integral(entity)));
	}

	auto Physics::to_entity(const void* user_data) noexcept -> entt::entity
	{
		return entt::entity{static_cast<std::underlying_type_t<entt::entity>>(std::bit_cast<std::uintptr_t>(user_data))};
	}

	auto Physics::physics_position_of(const sf::Vector2f position) noexcept -> b2Vec2
	{
		return to_physics(position);
	}

	auto Physics::position_of(const b2Vec2 physics_position) noexcept -> sf::Vector2f
	{
		return from_physics(physics_position);
	}

	auto Physics::physics_size_of(const sf::Vector2f size) noexcept -> b2Vec2
	{
		return to_physics(size);
	}

	auto Physics::size_of(const b2Vec2 physics_size) noexcept -> sf::Vector2f
	{
		return from_physics(physics_size);
	}

	auto Physics::physics_rotation_of(const sf::Angle angle) noexcept -> b2Rot
	{
		return b2MakeRot(angle.asRadians());
	}

	auto Physics::rotation_of(const b2Rot physics_angle) noexcept -> sf::Angle
	{
		const auto angle = b2Rot_GetAngle(physics_angle);

		return sf::radians(angle);
	}
}
