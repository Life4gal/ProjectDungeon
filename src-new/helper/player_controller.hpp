// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>

namespace pd::helper
{
	class PlayerController final
	{
	public:
		// ==========================================
		// 状态
		// ==========================================

		// 控制器是否正在控制任意实体
		[[nodiscard]] static auto online(entt::registry& registry) noexcept -> bool;

		// 获取控制器当前控制的实体
		// 如果控制器没有控制任何实体则返回null
		[[nodiscard]] static auto target(entt::registry& registry) noexcept -> entt::entity;

		// 指定控制器当前控制的实体
		static auto set_target(entt::registry& registry, entt::entity new_target) noexcept -> void;

		// ==========================================
		// 控制实体位置
		// ==========================================

		// 获取控制器当前控制的实体的位置
		// 如果控制器没有控制任何实体则返回{0,0}
		[[nodiscard]] static auto get_position(entt::registry& registry) noexcept -> sf::Vector2f;

		// 移动控制器控制的实体到指定的位置
		// 如果控制器没有控制任何实体则什么也不做
		static auto move_to(entt::registry& registry, sf::Vector2f new_position) noexcept -> void;

		// 平移控制器控制的实体
		// 如果控制器没有控制任何实体则什么也不做
		static auto translate(entt::registry& registry, sf::Vector2f distance) noexcept -> void;

		// ==========================================
		// 控制实体移动
		// ==========================================

		// TODO: 编写一套可用的输入管理器

		static auto left(entt::registry& registry) noexcept -> void;
		static auto right(entt::registry& registry) noexcept -> void;
		static auto stop_horizontal(entt::registry& registry) noexcept -> void;
		static auto up(entt::registry& registry) noexcept -> void;
		static auto down(entt::registry& registry) noexcept -> void;
		static auto stop_vertical(entt::registry& registry) noexcept -> void;
	};
}
