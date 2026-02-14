// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <span>

#include <config/flip_animation.hpp>

#include <entt/core/fwd.hpp>

namespace pd::blueprint
{
	// 一页翻页动画所包含的数据(对应config::FlipAnimationFrame)
	class FlipAnimationFrame final
	{
	public:
		// 该帧使用纹理id(通过systems::FlipAnimation::get(id)获取纹理资源)
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

	// 一个完整动画所包含的数据(对应config::flip_animation_frames_type)
	using flip_animation_frames_type = std::vector<FlipAnimationFrame>;

	// flip_animation_frames_type只读视图
	using flip_animation_frames_view_type = std::span<const FlipAnimationFrame>;

	// 所有动画所包含的数据(对应config::flip_animations_type)
	// 动画名称<->动画数据
	using flip_animations_type = std::unordered_map<
		std::string,
		flip_animation_frames_type,
		utility::StringHash,
		std::ranges::equal_to
	>;
}
