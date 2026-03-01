// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <box2d/id.h>
#include <box2d/math_functions.h>

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

		// 获取一个已经附加了物理刚体组件的实体的物理位置
		[[nodiscard]] static auto get_position(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> b2Vec2;

		// 设置一个已经附加了物理刚体组件的实体的物理位置
		static auto set_position(entt::registry& registry, entt::entity entity_with_physics_body, b2Vec2 position) noexcept -> void;

		// 获取一个已经附加了物理刚体组件的实体的物理旋转
		[[nodiscard]] static auto get_rotation(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> b2Rot;

		// 设置一个已经附加了物理刚体组件的实体的物理旋转
		static auto set_rotation(entt::registry& registry, entt::entity entity_with_physics_body, b2Rot rotation) noexcept -> void;

		// 获取一个已经附加了物理刚体组件的实体的物理变换
		[[nodiscard]] static auto get_transform(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> b2Transform;

		// 设置一个已经附加了物理刚体组件的实体的物理变换
		static auto set_transform(entt::registry& registry, entt::entity entity_with_physics_body, b2Transform transform) noexcept -> void;

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
