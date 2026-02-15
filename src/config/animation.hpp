// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <chrono>
#include <filesystem>
#include <vector>
#include <unordered_map>

#include <utility/string_hash.hpp>

namespace pd::config
{
	// 一帧动画所包含的数据
	class AnimationFrame final
	{
	public:
		// 该帧使用纹理路径(通常来说会使用同一张纹理)
		std::filesystem::path texture_path;
		// 该帧持续时间
		std::chrono::microseconds duration;
		// 该帧纹理位置
		int frame_x;
		int frame_y;
		// 该帧纹理大小
		int frame_width;
		int frame_height;
		// 总是假定原点为正中心(width/2, height/2)
	};

	// 完整动画所包含的数据
	class Animation final
	{
	public:
		using frames_type = std::vector<AnimationFrame>;

		// 所有帧
		frames_type frames;
		// 是否循环播放
		bool looping;
	};

	// 动画集
	// 动画名称<->动画数据
	class AnimationSet final : public std::unordered_map<
				std::string,
				Animation,
				utility::StringHash,
				std::ranges::equal_to
			>
	{
	public:
		using unordered_map::unordered_map;
	};

	// 从文件中读取动画数据
	[[nodiscard]] auto load_animation(const std::filesystem::path& path) noexcept -> AnimationSet;
}
