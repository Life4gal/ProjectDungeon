// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/initialize/asset.hpp>

#include <algorithm>
#include <ranges>

#include <systems/helper/animation.hpp>

#include <systems/helper/font_manager.hpp>
#include <systems/helper/texture_manager.hpp>
#include <systems/helper/sound_manager.hpp>
#include <systems/helper/music_manager.hpp>

#include <entt/entt.hpp>

namespace pd::systems::initialize
{
	auto asset(entt::registry& registry) noexcept -> void
	{
		registry.ctx().emplace<asset::FontManager>();
		registry.ctx().emplace<asset::TextureManager>();
		registry.ctx().emplace<asset::SoundManager>();
		registry.ctx().emplace<asset::MusicManager>();

		// 动画
		{
			const auto& set = helper::Animation::get_set(registry);

			// 载入用到的纹理资源
			std::ranges::for_each(
				set | std::views::values | std::views::transform(&blueprint::Animation::frames) | std::views::join,
				[&registry](const blueprint::AnimationFrame& frame) noexcept -> void
				{
					// 等我们使用ID而不是路径时这里的id就有用了
					[[maybe_unused]] const auto id = helper::TextureManager::load(registry, frame.texture_path);
				}
			);
		}
	}
} // namespace pd::systems::initialize
