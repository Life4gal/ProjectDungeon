// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/animation.hpp>

namespace pd::config
{
	[[nodiscard]] auto load_animation() noexcept -> AnimationSet
	{
		using namespace config;
		using namespace std::chrono_literals;

		AnimationSet animation_set{};

		// AntleredRascal
		{
			auto& [frames, looping] = animation_set["AntleredRascal"];

			// 循环播放
			looping = true;
			// 第1帧
			frames.emplace_back(
				AnimationFrame
				{
						.texture_path = "./assets/characters/enemies/deep-dive-AntleredRascal.png",
						.duration = 250ms,
						.frame_x = 0,
						.frame_y = 0,
						.frame_width = 16,
						.frame_height = 16,
				}
			);
			// 第2帧
			frames.emplace_back(
				AnimationFrame
				{
						.texture_path = "./assets/characters/enemies/deep-dive-AntleredRascal.png",
						.duration = 250ms,
						.frame_x = 16,
						.frame_y = 0,
						.frame_width = 16,
						.frame_height = 16,
				}
			);
			// 第3帧
			frames.emplace_back(
				AnimationFrame
				{
						.texture_path = "./assets/characters/enemies/deep-dive-AntleredRascal.png",
						.duration = 250ms,
						.frame_x = 32,
						.frame_y = 0,
						.frame_width = 16,
						.frame_height = 16,
				}
			);
			// 第4帧
			frames.emplace_back(
				AnimationFrame
				{
						.texture_path = "./assets/characters/enemies/deep-dive-AntleredRascal.png",
						.duration = 250ms,
						.frame_x = 48,
						.frame_y = 0,
						.frame_width = 16,
						.frame_height = 16,
				}
			);
		}

		return animation_set;
	}
}
