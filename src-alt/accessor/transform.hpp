// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

namespace pd::accessor
{
	class Transform final
	{
	public:
		// ===================================================

		// 初始化系统
		static auto initialize_system(entt::registry& registry) noexcept -> void;

		// 销毁系统
		static auto destroy_system(entt::registry& registry) noexcept -> void;

		// ===================================================

		// 为一个实体附加变换组件,如果目标实体已经存在变换组件则什么也不做
		static auto attach(entt::registry& registry, entt::entity entity, sf::Vector2f position, sf::Vector2f scale = sf::Vector2f{1, 1}, sf::Angle rotation = sf::degrees(0)) noexcept -> void;

		// 为一个实体附加变换组件,如果目标实体已经存在变换组件则会进行覆盖
		static auto attach_force(entt::registry& registry, entt::entity entity, sf::Vector2f position, sf::Vector2f scale = sf::Vector2f{1, 1}, sf::Angle rotation = sf::degrees(0)) noexcept -> void;

		// 为一个实体移除变换组件,如果目标实体没有变换组件则什么也不做
		static auto deattach(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 获取一个实体的位置,如果目标实体没有变换组件则返回{0,0}
		[[nodiscard]] static auto get_position(entt::registry& registry, entt::entity entity) noexcept -> sf::Vector2f;

		// 设置一个实体的位置,如果目标实体没有变换组件则什么也不做
		// 如果该实体存在物理刚体组件则此接口没有意义,因为带物理刚体组件的变换组件的位置是只读的,其随物理刚体的位置自动更新
		static auto set_position(entt::registry& registry, entt::entity entity, sf::Vector2f new_position) noexcept -> void;

		// 获取一个实体的缩放,如果目标实体没有变换组件则返回{1,1}
		[[nodiscard]] static auto get_scale(entt::registry& registry, entt::entity entity) noexcept -> sf::Vector2f;

		// 设置一个实体的缩放,如果目标实体没有变换组件则什么也不做
		static auto set_scale(entt::registry& registry, entt::entity entity, sf::Vector2f new_scale) noexcept -> void;

		// 获取一个实体的旋转,如果目标实体没有变换组件则返回0度
		[[nodiscard]] static auto get_rotation(entt::registry& registry, entt::entity entity) noexcept -> sf::Angle;

		// 设置一个实体的旋转,如果目标实体没有变换组件则什么也不做
		// 如果该实体存在物理刚体组件则此接口没有意义,因为带物理刚体组件的变换组件的旋转是只读的,其随物理刚体的旋转自动更新
		static auto set_rotation(entt::registry& registry, entt::entity entity, sf::Angle new_rotation) noexcept -> void;
	};
}
