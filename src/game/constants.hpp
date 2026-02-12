// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/core/hashed_string.hpp>

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

		// constexpr static int window_width = 1920;
		// constexpr static int window_height = 1080;

		// ===================================================================
		// ECS NAME
		// EnTT不允许registry.ctx.emplace<T>()的T有重复类型,而每次都定义一个类型进行包装会显得繁琐
		// 故使用名字区分相同类型的数据
		// ===================================================================

		// =======================
		// asset::map_type
		// 不同资源使用不同map,我们基于name选择对应的map
		// =======================

		// 字体资源
		constexpr static auto asset_font = entt::hashed_string{"AssetFont"};
		// 纹理资源
		constexpr static auto asset_texture = entt::hashed_string{"AssetTexture"};
		// 音效资源
		constexpr static auto asset_sound = entt::hashed_string{"AssetSound"};
		// 音乐资源
		constexpr static auto asset_music = entt::hashed_string{"AssetMusic"};

		// =======================
		// blueprint::animations_type
		// =======================

		// 动画资源
		constexpr static auto blueprint_animation = entt::hashed_string{"BlueprintAnimation"};

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
