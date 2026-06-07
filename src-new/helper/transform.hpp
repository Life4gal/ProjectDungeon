// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <span>

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

namespace pd::helper
{
	class Transform final
	{
	public:
		// ================================================
		// GETTER & SETTER
		// ================================================

		// 获取一个实体的位置,如果目标实体没有变换组件则返回{0,0}
		[[nodiscard]] static auto get_position(entt::registry& registry, entt::entity entity) noexcept -> sf::Vector2f;

		// 设置一个实体的位置,如果目标实体没有变换组件则什么也不做
		// 如果该实体存在物理刚体组件则此接口没有意义,因为带物理刚体组件的变换组件的位置是只读的,其随物理刚体的位置自动更新
		static auto set_position(entt::registry& registry, entt::entity entity, sf::Vector2f new_position) noexcept -> void;

		// 平移一个实体的位置,如果目标实体没有变换组件则什么也不做
		// 如果该实体存在物理刚体组件则此接口没有意义,因为带物理刚体组件的变换组件的位置是只读的,其随物理刚体的位置自动更新
		static auto translate(entt::registry& registry, entt::entity entity, sf::Vector2f distance) noexcept -> void;

		// 获取一个实体的旋转,如果目标实体没有变换组件则返回0度
		[[nodiscard]] static auto get_rotation(entt::registry& registry, entt::entity entity) noexcept -> sf::Angle;

		// 设置一个实体的旋转,如果目标实体没有变换组件则什么也不做
		// 如果该实体存在物理刚体组件则此接口没有意义,因为带物理刚体组件的变换组件的旋转是只读的,其随物理刚体的旋转自动更新
		static auto set_rotation(entt::registry& registry, entt::entity entity, sf::Angle new_rotation) noexcept -> void;
	};
}
