// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string_view>

#include <SFML/System/Vector2.hpp>

#include <box2d/math_functions.h>

namespace pd
{
	class Constant final
	{
	public:
		// ===================================================================
		// WINDOW
		// ===================================================================

		constexpr static int window_width = 1920;
		constexpr static int window_height = 1080;
		constexpr static std::string_view window_title{"ProjectDungeon"};

		// ===================================================================
		// PHYSICS WORLD
		// ===================================================================

		// 多少像素等价于物理世界的一米
		constexpr static auto pixels_per_meter = 32.f;
		// 一个像素等价于物理世界多少米
		constexpr static auto meters_per_pixel = 1.f / pixels_per_meter;

		// 物理世界数值转为像素世界
		[[nodiscard]] constexpr static auto from_physics(const float physics) noexcept -> float
		{
			return pixels_per_meter * physics;
		}

		// 物理世界数值转为像素世界
		[[nodiscard]] /*constexpr*/ static auto from_physics(const b2Vec2 physics) noexcept -> sf::Vector2f
		{
			const auto p = pixels_per_meter * physics;
			return {p.x, p.y};
		}

		// 物理世界数值转为像素世界
		[[nodiscard]] constexpr static auto from_physics(const sf::Vector2f physics) noexcept -> sf::Vector2f
		{
			const auto p = pixels_per_meter * physics;
			return {p.x, p.y};
		}

		// 像素世界数值转为物理世界
		[[nodiscard]] constexpr static auto to_physics(const float pixels) noexcept -> float
		{
			return meters_per_pixel * pixels;
		}

		// 像素世界数值转为物理世界
		[[nodiscard]] /*constexpr*/ static auto to_physics(const sf::Vector2f pixels) noexcept -> b2Vec2
		{
			const auto p = meters_per_pixel * pixels;
			return {p.x, p.y};
		}

		// 像素世界数值转为物理世界
		[[nodiscard]] /*constexpr*/ static auto to_physics(const b2Vec2 pixels) noexcept -> b2Vec2
		{
			return meters_per_pixel * pixels;
		}
	};
} // namespace pd
