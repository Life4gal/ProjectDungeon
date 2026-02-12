// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <unordered_map>

namespace pd::config
{
	// 一帧动画所包含的数据
	class AnimationFrame final
	{
	public:
		// 该帧使用纹理路径
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

	// 一个完整动画所包含的数据
	using animation_frames_type = std::vector<AnimationFrame>;

	// 所有动画所包含的数据
	// 动画名称<->动画数据
	using animations_type = std::unordered_map<std::string, animation_frames_type>;
}
