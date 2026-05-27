// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <span>
#include <vector>

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>

#include <box2d/id.h>
#include <box2d/math_functions.h>

namespace pd::helper
{
	class Collision final
	{
	public:
// ================================================
		// GETTER & SETTER
		// ================================================

		// 禁用一个实体的物理刚体,如果目标实体没有物理刚体组件则什么也不做
		static auto disable(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 启用一个实体的物理刚体,如果目标实体没有物理刚体组件则什么也不做
		static auto enable(entt::registry& registry, entt::entity entity) noexcept -> void;

		// ==================
		// POSITION
		// ==================

		// 获取一个实体的物理刚体的位置,如果目标实体没有物理刚体组件则返回b2Vec2_zero
		// 使用Transform::get_position更高效,因为它不需要调用b2Body_GetPosition
		[[nodiscard]] static auto get_position(entt::registry& registry, entt::entity entity) noexcept -> b2Vec2;

		// 设置一个实体的物理刚体的位置,如果目标实体没有物理刚体组件则什么也不做
		static auto set_position(entt::registry& registry, entt::entity entity, b2Vec2 new_position) noexcept -> void;

		// 获取一个实体的物理刚体的位置(像素),如果目标实体没有物理刚体组件则返回{0,0}
		// 使用Transform::get_position更高效,因为它不需要调用b2Body_GetPosition
		[[nodiscard]] static auto get_pixel_position(entt::registry& registry, entt::entity entity) noexcept -> sf::Vector2f;

		// 设置一个实体的物理刚体的位置(像素),如果目标实体没有物理刚体组件则什么也不做
		static auto set_pixel_position(entt::registry& registry, entt::entity entity, sf::Vector2f new_position) noexcept -> void;

		// 获取一个实体的物理刚体的位置(像素,基于屏幕(相机)位置),如果目标实体没有物理刚体组件则返回{0,0}
		// 使用Transform::get_screen_position更高效,因为它不需要调用b2Body_GetPosition,也不需要获取屏幕(相机)位置
		[[nodiscard]] static auto get_screen_position(entt::registry& registry, entt::entity entity) noexcept -> sf::Vector2i;

		// 获取多个实体的物理刚体的位置(像素,基于屏幕(相机)位置),如果目标实体没有物理刚体组件则返回{0,0}
		// 使用Transform::get_screen_position更高效,因为它不需要调用b2Body_GetPosition,也不需要获取屏幕(相机)位置
		// 相比于对每个实体调用一次get_screen_position,此接口效率要更高,因为无需多次访问registry来获取屏幕(相机)位置
		[[nodiscard]] static auto get_screen_position(entt::registry& registry, std::span<const entt::entity> entities) noexcept -> std::vector<sf::Vector2i>;

		// 设置一个实体的物理刚体的位置(像素,基于屏幕(相机)位置),如果目标实体没有物理刚体组件则什么也不做
		static auto set_screen_position(entt::registry& registry, entt::entity entity, sf::Vector2i new_position) noexcept -> void;

		// 设置多个实体的物理刚体的位置(像素,基于屏幕(相机)位置),如果目标实体没有物理刚体组件则什么也不做
		// 相比于对每个实体调用一次set_screen_position,此接口效率要更高,因为无需多次访问registry来获取屏幕(相机)位置
		static auto set_screen_position(entt::registry& registry, std::span<const entt::entity> entities, std::span<const sf::Vector2i> new_positions) noexcept -> void;

		// 平移一个实体的物理刚体的位置,如果目标实体没有物理刚体组件则什么也不做
		static auto translate(entt::registry& registry, entt::entity entity, b2Vec2 distance) noexcept -> void;

		// 平移一个实体的物理刚体的位置(像素),如果目标实体没有物理刚体组件则什么也不做
		static auto translate_pixel(entt::registry& registry, entt::entity entity, sf::Vector2f distance) noexcept -> void;

		// ==================
		// ROTATION
		// ==================

		// 获取一个实体的物理刚体的旋转,如果目标实体没有物理刚体组件则返回b2Rot_identity
		// 使用Transform::get_rotation更高效,因为它不需要调用b2Body_GetRotation
		[[nodiscard]] static auto get_rotation(entt::registry& registry, entt::entity entity) noexcept -> b2Rot;

		// 设置一个实体的物理刚体的旋转,如果目标实体没有物理刚体组件则什么也不做
		static auto set_rotation(entt::registry& registry, entt::entity entity, b2Rot new_rotation) noexcept -> void;

		// ==================
		// TRANSFORM
		// ==================

		// 获取一个实体的物理刚体的变换,如果目标实体没有物理刚体组件则返回b2Transform_identity
		// 使用Transform::get_position+Transform::get_rotation更高效,因为它不需要调用b2Body_GetTransform
		[[nodiscard]] static auto get_transform(entt::registry& registry, entt::entity entity) noexcept -> b2Transform;

		// 设置一个实体的物理刚体的变换,如果目标实体没有物理刚体组件则什么也不做
		static auto set_transform(entt::registry& registry, entt::entity entity, b2Transform new_transform) noexcept -> void;

		// ==================
		// SCALE
		// ==================

		// TODO: 缩放如何实现?

		// ==================
		// VELOCITY
		// ==================

		// 获取一个实体的物理刚体的线性速度,如果目标实体没有物理刚体组件则返回b2Vec2_zero
		[[nodiscard]] static auto get_linear_velocity(entt::registry& registry, entt::entity entity) noexcept -> b2Vec2;

		// 获取一个物理刚体的线性速度
		[[nodiscard]] static auto get_linear_velocity(b2BodyId body_id) noexcept -> b2Vec2;

		// 设置一个实体的物理刚体的线性速度,如果目标实体没有物理刚体组件则什么也不做
		static auto set_linear_velocity(entt::registry& registry, entt::entity entity, b2Vec2 new_velocity) noexcept -> void;

		// 设置一个物理刚体的线性速度
		static auto set_linear_velocity(b2BodyId body_id, b2Vec2 new_velocity) noexcept -> void;
	};
}
