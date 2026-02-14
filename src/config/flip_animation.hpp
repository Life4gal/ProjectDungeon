// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <unordered_map>

#include <utility/string_hash.hpp>

namespace pd::config
{
	// 一页翻页动画所包含的数据
	class FlipAnimationFrame final
	{
	public:
		// 该帧使用纹理路径(通常来说会使用同一张纹理)
		std::string texture_path;
		// 该帧持续时间
		std::chrono::microseconds duration;
		// 该帧纹理位置
		int frame_x;
		int frame_y;
		// 该帧纹理大小
		int frame_width;
		int frame_height;
	};

	// 一个完整翻页动画所包含的数据
	using flip_animation_frames_type = std::vector<FlipAnimationFrame>;

	// 所有翻页动画所包含的数据
	// 动画名称<->动画数据
	using flip_animations_type = std::unordered_map<
		std::string,
		flip_animation_frames_type,
		utility::StringHash,
		std::ranges::equal_to
	>;
}
