// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace pd::helper
{
	class Camera final
	{
	public:
		// ================================================
		// UPDATE
		// ================================================

		// 相机是否有变动(需要重新计算相机区域内实体)
		[[nodiscard]] static auto dirty(entt::registry& registry) noexcept -> bool;

		// 标记相机已更新
		static auto clean(entt::registry& registry) noexcept -> void;

		// ================================================
		// GETTER & SETTER
		// ================================================

		// 初始化相机区域
		// 不发出事件
		static auto initialize(entt::registry& registry, sf::FloatRect area) noexcept -> void;

		// 获取相机区域
		[[nodiscard]] static auto get_area(entt::registry& registry) noexcept -> sf::FloatRect;

		// 设置相机区域
		//
		// Event:
		//  event::camera::SetArea
		static auto set_area(entt::registry& registry, sf::FloatRect new_area) noexcept -> void;

		// 获取相机位置
		[[nodiscard]] static auto get_position(entt::registry& registry) noexcept -> sf::Vector2f;

		// 设置相机位置
		//
		// Event:
		//  event::camera::Move
		static auto set_position(entt::registry& registry, sf::Vector2f new_position) noexcept -> void;

		// 平移相机
		// = set_position(get_position() + distance)
		//
		// Event:
		//  event::camera::Translate
		static auto translate(entt::registry& registry, sf::Vector2f distance) noexcept -> void;

		// 获取相机区域大小
		[[nodiscard]] static auto get_size(entt::registry& registry) noexcept -> sf::Vector2f;

		// 设置相机区域大小
		//
		// Event:
		//  event::camera::Resize
		static auto set_size(entt::registry& registry, sf::Vector2f new_size) noexcept -> void;
	};
}
