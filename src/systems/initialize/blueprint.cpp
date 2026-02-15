// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/initialize/blueprint.hpp>

#include <array>
#include <algorithm>
#include <ranges>

#include <ctx/animation.hpp>

#include <entt/entt.hpp>

namespace pd::systems::initialize
{
	auto blueprint(entt::registry& registry) noexcept -> void
	{
		// 动画
		{
			const std::array paths
			{
					std::filesystem::path("assets/configs/animations.json"),
					// ...
			};

			std::ranges::for_each(
				paths,
				[&](const auto& path) noexcept-> void
				{
					// 读取数据
					const auto animation_config = config::load_animation(path);

					// 载入蓝图
					auto animation_blueprint = blueprint::load_animation(animation_config);

					// 合并动画
					ctx::Animation::add_set(registry, std::move(animation_blueprint));
				}
			);
		}
	}
}
