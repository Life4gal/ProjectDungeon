// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <components/physics_body.hpp>

#include <entt/fwd.hpp>

#include <box2d/math_functions.h>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

struct b2BodyDef;
struct b2ShapeDef;

struct b2Circle;
struct b2Capsule;
struct b2Polygon;

namespace pd::attacher
{
	class PhysicsBody final
	{
	public:
		class ShapeAttacher final
		{
			friend PhysicsBody;

			b2BodyId body_id_;

			constexpr explicit ShapeAttacher(const b2BodyId body_id) noexcept
				: body_id_{body_id} {}

		public:
			// 为指定刚体附加一个圆形碰撞体
			auto attach(const b2ShapeDef& shape_def, const b2Circle& circle) const noexcept -> b2ShapeId;

			// 为指定刚体附加一个胶囊形碰撞体
			auto attach(const b2ShapeDef& shape_def, const b2Capsule& capsule) const noexcept -> b2ShapeId;

			// 为指定刚体附加一个多边形碰撞体
			auto attach(const b2ShapeDef& shape_def, const b2Polygon& polygon) const noexcept -> b2ShapeId;
		};

		// 为一个实体附加一个物理刚体,如果该实体已经有物理刚体则先销毁原有的物理刚体然后覆盖
		static auto attach(entt::registry& registry, entt::entity entity, b2WorldId world_id, const b2BodyDef& body_def) noexcept -> ShapeAttacher;

		// 为一个实体移除物理刚体组件,如果目标实体没有物理刚体组件则什么也不做
		static auto deattach(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 禁用一个实体的物理刚体,如果目标实体没有物理刚体组件则什么也不做
		static auto disable(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 启用一个实体的物理刚体,如果目标实体没有物理刚体组件则什么也不做
		static auto enable(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 获取一个实体的物理刚体的位置,如果目标实体没有物理刚体组件则返回b2Vec2_zero
		// 使用Transform::get_position更高效,因为它不需要调用b2Body_GetPosition
		[[nodiscard]] static auto get_position(entt::registry& registry, entt::entity entity) noexcept -> b2Vec2;

		// 设置一个实体的物理刚体的位置,如果目标实体没有物理刚体组件则什么也不做
		static auto set_position(entt::registry& registry, entt::entity entity, b2Vec2 new_position) noexcept -> void;

		// 获取一个实体的物理刚体的旋转,如果目标实体没有物理刚体组件则返回b2Rot_identity
		// 使用Transform::get_rotation更高效,因为它不需要调用b2Body_GetRotation
		[[nodiscard]] static auto get_rotation(entt::registry& registry, entt::entity entity) noexcept -> b2Rot;

		// 设置一个实体的物理刚体的旋转,如果目标实体没有物理刚体组件则什么也不做
		static auto set_rotation(entt::registry& registry, entt::entity entity, b2Rot new_rotation) noexcept -> void;

		// 获取一个实体的物理刚体的变换,如果目标实体没有物理刚体组件则返回b2Transform_identity
		// 使用Transform::get_position+Transform::get_rotation更高效,因为它不需要调用b2Body_GetTransform
		[[nodiscard]] static auto get_transform(entt::registry& registry, entt::entity entity) noexcept -> b2Transform;

		// 设置一个实体的物理刚体的变换,如果目标实体没有物理刚体组件则什么也不做
		static auto set_transform(entt::registry& registry, entt::entity entity, b2Transform new_transform) noexcept -> void;

		// ============================
		// 提供一些数值转换接口用于将像素单位转为物理单位(反正亦然)
		// ============================

		[[nodiscard]] static auto to_user_data(entt::entity entity) noexcept -> void*;

		[[nodiscard]] static auto to_entity(const void* user_data) noexcept -> entt::entity;

		// 像素位置转为物理位置
		[[nodiscard]] static auto physics_position_of(sf::Vector2f position) noexcept -> b2Vec2;

		// 物理位置转为像素位置
		[[nodiscard]] static auto position_of(b2Vec2 physics_position) noexcept -> sf::Vector2f;

		// 像素大小转为物理大小
		[[nodiscard]] static auto physics_size_of(sf::Vector2f size) noexcept -> b2Vec2;

		// 物理大小转为像素大小
		[[nodiscard]] static auto size_of(b2Vec2 physics_size) noexcept -> sf::Vector2f;

		// 像素旋转转为物理旋转
		[[nodiscard]] static auto physics_rotation_of(sf::Angle angle) noexcept -> b2Rot;

		// 物理旋转转为像素旋转
		[[nodiscard]] static auto rotation_of(b2Rot physics_angle) noexcept -> sf::Angle;
	};
}
