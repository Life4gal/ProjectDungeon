// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include<blueprint/animation.hpp>

#include <config/animation.hpp>
#include <asset/map.hpp>

namespace pd::blueprint
{
	auto load_animation(const config::AnimationSet& in_animation_set, asset::TextureMap& out_texture_map) noexcept -> AnimationSet
	{
		AnimationSet animation_set{};

		animation_set.reserve(in_animation_set.size());
		// 具体纹理数量未知,不过map不reserve问题也不大

		for (auto&& [animation_name, animation_frames]: in_animation_set /* | std::views::as_rvalue*/)
		{
			Animation animation{};

			animation.looping = animation_frames.looping;
			animation.frames.reserve(animation_frames.frames.size());

			for (auto&& frame: animation_frames.frames /* | std::views::as_rvalue*/)
			{
				// 注册纹理
				const auto texture_id = out_texture_map.id_of(frame.texture_path);

				animation.frames.emplace_back(
					texture_id,
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
