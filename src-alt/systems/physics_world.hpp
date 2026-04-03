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
		// 创建物理世界
		static auto create(entt::registry& registry) noexcept -> void;

		// 销毁物理世界
		static auto destroy(entt::registry& registry) noexcept -> void;

		// 物理世界id
		[[nodiscard]] static auto id(entt::registry& registry) noexcept -> b2WorldId;

		// ============================
		// 数值转换
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

		// ============================
		// 附加形状
		// ============================

		// 为指定刚体附加一个圆形碰撞体,假定刚体有效
		static auto attach_shape(b2BodyId body_id, const b2ShapeDef& shape_def, const b2Circle& circle) noexcept -> b2ShapeId;

		// 为指定刚体附加一个胶囊形碰撞体,假定刚体有效
		static auto attach_shape(b2BodyId body_id, const b2ShapeDef& shape_def, const b2Capsule& capsule) noexcept -> b2ShapeId;

		// 为指定刚体附加一个多边形碰撞体,假定刚体有效
		static auto attach_shape(b2BodyId body_id, const b2ShapeDef& shape_def, const b2Polygon& polygon) noexcept -> b2ShapeId;

		// ============================
		// 刚体
		// ============================

		// 为一个实体附加一个物理体,如果该实体已经有物理体则先销毁原有的物理体(直接使用返回的body_id要比通过实体调用attach_shape更高效)
		static auto attach(entt::registry& registry, entt::entity entity, b2WorldId world_id, const b2BodyDef& body_def) noexcept -> b2BodyId;

		// 销毁一个实体的物理体,如果该实体没有物理体则什么都不做
		// static auto deattach(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> void;
		static auto deattach(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 禁用一个实体的物理体,如果该实体没有物理体则什么都不做
		// static auto disable(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> void;
		static auto disable(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 启用一个实体的物理体,如果该实体没有物理体则什么都不做
		// static auto enable(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> void;
		static auto enable(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 获取一个实体的物理体的位置,如果该实体没有物理体则返回b2Vec2_zero
		// [[nodiscard]] static auto get_position(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> b2Vec2;
		[[nodiscard]] static auto get_position(entt::registry& registry, entt::entity entity) noexcept -> b2Vec2;

		// 设置一个实体的物理体的位置,如果该实体没有物理体则什么都不做
		// static auto set_position(entt::registry& registry, entt::entity entity_with_physics_body, b2Vec2 position) noexcept -> void;
		static auto set_position(entt::registry& registry, entt::entity entity, b2Vec2 position) noexcept -> void;

		// 获取一个实体的物理体的旋转,如果该实体没有物理体则返回b2Rot_identity
		// [[nodiscard]] static auto get_rotation(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> b2Rot;
		[[nodiscard]] static auto get_rotation(entt::registry& registry, entt::entity entity) noexcept -> b2Rot;

		// 设置一个实体的物理体的旋转,如果该实体没有物理体则什么都不做
		// static auto set_rotation(entt::registry& registry, entt::entity entity_with_physics_body, b2Rot rotation) noexcept -> void;
		static auto set_rotation(entt::registry& registry, entt::entity entity, b2Rot rotation) noexcept -> void;

		// 获取一个实体的物理体的变换,如果该实体没有物理体则返回b2Transform_identity
		// [[nodiscard]] static auto get_transform(entt::registry& registry, entt::entity entity_with_physics_body) noexcept -> b2Transform;
		[[nodiscard]] static auto get_transform(entt::registry& registry, entt::entity entity) noexcept -> b2Transform;

		// 设置一个实体的物理体的变换,如果该实体没有物理体则什么都不做
		// static auto set_transform(entt::registry& registry, entt::entity entity_with_physics_body, b2Transform transform) noexcept -> void;
		static auto set_transform(entt::registry& registry, entt::entity entity, b2Transform transform) noexcept -> void;

		// ============================
		// 形状
		// ============================

		// 为一个已经附加了刚体的实体附加一个圆形碰撞体,如果该实体没有刚体则什么都不做(返回b2_nullShapeId)
		// static auto attach_shape(entt::registry& registry, entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Circle& circle) noexcept -> b2ShapeId;
		static auto attach_shape(entt::registry& registry, entt::entity entity, const b2ShapeDef& shape_def, const b2Circle& circle) noexcept -> b2ShapeId;

		// 为一个已经附加了刚体的实体附加一个胶囊形碰撞体,如果该实体没有刚体则什么都不做(返回b2_nullShapeId)
		// static auto attach_shape(entt::registry& registry, entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Capsule& capsule) noexcept -> b2ShapeId;
		static auto attach_shape(entt::registry& registry, entt::entity entity, const b2ShapeDef& shape_def, const b2Capsule& capsule) noexcept -> b2ShapeId;

		// 为一个已经附加了刚体的实体附加一个多边形碰撞体,如果该实体没有刚体则什么都不做(返回b2_nullShapeId)
		// static auto attach_shape(entt::registry& registry, entt::entity entity_with_physics_body, const b2ShapeDef& shape_def, const b2Polygon& polygon) noexcept -> b2ShapeId;
		static auto attach_shape(entt::registry& registry, entt::entity entity, const b2ShapeDef& shape_def, const b2Polygon& polygon) noexcept -> b2ShapeId;
	};
}
