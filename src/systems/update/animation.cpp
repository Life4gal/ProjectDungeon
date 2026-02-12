// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/update/animation.hpp>

#include <components/animation.hpp>

#include <entt/entt.hpp>

namespace pd::systems::update
{
	auto animation(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		using namespace components;

		for (const auto view = registry.view<AnimationTimer, AnimationIndex>();
		     const auto [entity, timer, index]: view.each())
		{
			timer.elapsed += delta;
			if (timer.elapsed < timer.duration)
			{
				continue;
			}

			timer.elapsed -= timer.duration;
			index.current += 1;

			if (index.current < index.total)
			{
				continue;
			}

			// 循环动画
			index.current %= index.total;
		}
	}
}
