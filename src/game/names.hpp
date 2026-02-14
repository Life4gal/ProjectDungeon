// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/core/hashed_string.hpp>

namespace pd
{
	class Names final
	{
	public:
		// ===================================================================
		// ECS NAME
		// EnTT不允许registry.ctx.emplace<T>()的T有重复类型,而每次都定义一个类型进行包装会显得繁琐
		// 故使用名字区分相同类型的数据
		// ===================================================================

		// ===============
		// asset::map_type 依赖下面的名字区分
		// ===============

		// 字体资源
		constexpr static auto asset_font = entt::hashed_string{"AssetFont"};
		// 纹理资源
		constexpr static auto asset_texture = entt::hashed_string{"AssetTexture"};
		// 音效资源
		constexpr static auto asset_sound = entt::hashed_string{"AssetSound"};
		// 音乐资源
		constexpr static auto asset_music = entt::hashed_string{"AssetMusic"};

		// ===============
		// 
		// ===============

		// 翻页动画资源
		constexpr static auto blueprint_flip_animation = entt::hashed_string{"BlueprintFlipAnimation"};
	};
}
