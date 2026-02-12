// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <config/animation.hpp>

#include <entt/core/fwd.hpp>

namespace pd::blueprint
{
	// 一帧动画所包含的数据(对应config::AnimationFrame)
	class AnimationFrame final
	{
	public:
		// 该帧使用纹理id(通过asset::AssetLoader::get(id)获取纹理资源)
		entt::id_type texture;
		// 该帧持续时间
		std::chrono::microseconds duration;
		// 该帧纹理位置
		int frame_x;
		int frame_y;
		// 该帧纹理大小
		int frame_width;
		int frame_height;
	};

	// 一个完整动画所包含的数据(对应config::animation_frames_type)
	using animation_frames_type = std::vector<AnimationFrame>;

	// 所有动画所包含的数据(对应config::animations_type)
	// 动画名称<->动画数据
	using animations_type = std::unordered_map<std::string, animation_frames_type>;
}
