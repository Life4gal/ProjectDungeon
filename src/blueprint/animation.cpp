// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include<blueprint/animation.hpp>

#include <config/animation.hpp>

namespace pd::blueprint
{
	auto load_animation(const config::AnimationSet& data) noexcept -> AnimationSet
	{
		AnimationSet animation_set{};

		animation_set.reserve(data.size());
		// 具体纹理数量未知,不过map不reserve问题也不大

		for (auto&& [animation_name, animation_frames]: data /* | std::views::as_rvalue*/)
		{
			Animation animation{};

			animation.looping = animation_frames.looping;
			animation.frames.reserve(animation_frames.frames.size());

			for (auto&& frame: animation_frames.frames /* | std::views::as_rvalue*/)
			{
				// 注册纹理
				// const auto texture_id = texture_map.id_of(frame.texture_path);

				animation.frames.emplace_back(
					frame.texture_path,
					frame.duration,
					frame.frame_x,
					frame.frame_y,
					frame.frame_width,
					frame.frame_height
				);
			}

			animation_set.emplace(animation_name, std::move(animation));
		}

		return animation_set;
	}
}
