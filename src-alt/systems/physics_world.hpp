// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <box2d/id.h>
#include <box2d/math_functions.h>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

struct b2BodyDef;
struct b2ShapeDef;

struct b2Circle;
struct b2Capsule;
struct b2Polygon;

namespace pd::systems
{
	class PhysicsWorld final
	{
	public:
		static auto create(entt::registry& registry) noexcept -> void;

		static auto destroy(entt::registry& registry) noexcept -> void;

		[[nodiscard]] static auto id(entt::registry& registry) noexcept -> b2WorldId;

		// ============================
		// 数值转换
		// ============================

		[[nodiscard]] static auto to_user_data(entt::entity entity) noexcept -> void*;

		[[nodiscard]] static auto to_entity(const void* user_data) noexcept -> entt::entity;

		[[nodiscard]] static auto physics_position_of(sf::Vector2f position) noexcept -> b2Vec2;

		[[nodiscard]] static auto position_of(b2Vec2 physics_position) noexcept -> sf::Vector2f;

		[[nodiscard]] static auto physics_size_of(sf::Vector2f size) noexcept -> b2Vec2;

		[[nodiscard]] static auto size_of(b2Vec2 physics_size) noexcept -> sf::Vector2f;

		[[nodiscard]] static auto physics_rotation_of(sf::Angle angle) noexcept -> b2Rot;

		[[nodiscard]] static auto rotation_of(b2Rot physics_angle) noexcept -> sf::Angle;

		// ============================
		// 实体
		// ============================

		static auto attach(entt::registry& registry, entt::entity entity, b2WorldId world_id, const b2BodyDef& body_def) noexcept -> void;

		static auto deattach(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> void;

		static auto disable(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> void;

		static auto enable(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> void;

		[[nodiscard]] static auto get_position(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> b2Vec2;

		static auto set_position(entt::registry& registry, entt::entity entity_with_physics_body, b2Vec2 position) noexcept -> void;

		[[nodiscard]] static auto get_rotation(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> b2Rot;

		static auto set_rotation(entt::registry& registry, entt::entity entity_with_physics_body, b2Rot rotation) noexcept -> void;

		[[nodiscard]] static auto get_transform(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> b2Transform;

		static auto set_transform(entt::registry& registry, entt::entity entity_with_physics_body, b2Transform transform) noexcept -> void;

		static auto attach_shape(entt::registry& registry, entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Circle& circle) noexcept -> void;

		static auto attach_shape(entt::registry& registry, entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Capsule& capsule) noexcept -> void;

		static auto attach_shape(entt::registry& registry, entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Polygon& polygon) noexcept -> void;
	};
}
