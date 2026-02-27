// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/update/animation.hpp>

#include <config/animation.hpp>

#include <components/animation.hpp>
#include <components/render.hpp>

#include <systems/helper/world.hpp>
#include <systems/helper/animation.hpp>

#include <entt/entt.hpp>

namespace pd::systems::update
{
	auto animation(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		using namespace components;
		using namespace systems;

		// 如果暂停游戏则无需更新动画
		if (helper::World::is_pause(registry))
		{
			return;
		}

		for (const auto view = registry.view<
			     animation::Timer,
			     animation::Index,
			     animation::Animation,
			     render::AnimationFrame
		     >(
			     entt::exclude<
				     // 如果动画暂停则无需更新动画
				     animation::Paused,
				     // 如果动画已经结束(例如非循环动画播完了最后一帧)则无需更新动画
				     animation::Ended
			     >
		     );
		     const auto [entity, timer, index, animation, animation_frame]: view.each())
		{
			timer.elapsed += delta;
			if (timer.elapsed < timer.duration)
			{
				continue;
			}

			timer.elapsed -= timer.duration;
			index.current += 1;

			// 如果已经到了最后一帧且不是循环动画则标记为已结束
			if (index.current >= index.total)
			{
				if (not registry.all_of<animation::Looping>(entity))
				{
					registry.emplace<animation::Ended>(entity);
					continue;
				}
			}

			// 循环
			index.current %= index.total;

			// 设置渲染纹理
			const auto& frame = animation.animation.get().frames[index.current];
			animation_frame.animation_frame = std::cref(frame);
		}
	}
}
