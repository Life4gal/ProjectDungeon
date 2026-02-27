// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <box2d/id.h>

#include <SFML/System/Vector2.hpp>

// ReSharper disable CppInconsistentNaming

struct b2BodyDef;
struct b2ShapeDef;

struct b2Circle;
struct b2Capsule;
struct b2Polygon;

// ReSharper restore CppInconsistentNaming

namespace pd::systems::helper
{
	class PhysicsBody final
	{
	public:
		// =============================================
		// BODY
		// =============================================

		// 为一个实体附加物理刚体组件
		// 如果目标实体已经存在物理刚体组件则会(先销毁)进行覆盖
		static auto attach(
			entt::registry& registry,
			entt::entity entity,
			b2WorldId world_id,
			const b2BodyDef& body_def
		) noexcept -> void;

		// 为一个实体移除物理刚体组件
		static auto deattach(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> void;

		// =============================================
		// SHAPE
		// =============================================

		// 为一个已经附加了物理刚体组件的实体附加碰撞体组件(圆形)
		static auto attach_shape(entt::registry& registry, entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Circle& circle) noexcept -> void;

		// 为一个已经附加了物理刚体组件的实体附加碰撞体组件(胶囊形)
		static auto attach_shape(entt::registry& registry, entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Capsule& capsule) noexcept -> void;

		// 为一个已经附加了物理刚体组件的实体附加碰撞体组件(多边形)
		static auto attach_shape(entt::registry& registry, entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Polygon& polygon) noexcept -> void;

		// 为一个已经附加了物理刚体组件的实体移除碰撞体组件
		static auto deattach_shape(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> void;

		// =============================================
		// 
		// =============================================

		static auto apply_force_to_center(entt::registry& registry, entt::entity entity_with_physics_body, sf::Vector2f factor) noexcept -> void;
	};
}
