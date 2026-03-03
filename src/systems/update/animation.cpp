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
#include <systems/helper/render.hpp>

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
			     animation::Animation,
			     // 我们并不在这里使用该组件,不过该组件帮助我们过滤那些没有渲染组件的实体(因为没有渲染组件的实体不需要更新动画帧)
			     render::AnimationFrame
		     >(
			     entt::exclude<
				     // 如果动画暂停则无需更新动画
				     animation::Paused,
				     // 如果动画已经结束(例如非循环动画播完了最后一帧)则无需更新动画
				     animation::Ended
			     >
		     );
		     const auto [entity, timer, animation, animation_frame]: view.each())
		{
			timer.elapsed += delta;
			// 如果此帧未结束,无需更新
			if (timer.elapsed < timer.duration)
			{
				continue;
			}

			// 帧计时并不重置为0,而是减去当前帧的持续时间
			timer.elapsed -= timer.duration;

			// 跳转到下一帧(因为我们已经设置了帧计时器,不需要再设置了)
			const auto next_frame_index = helper::Animation::jump_to_next_frame(registry, entity);
			if (next_frame_index == helper::Animation::animation_ended)
			{
				// 播放到了最后一帧且不是循环动画
				continue;
			}

			// 设置渲染纹理
			helper::Render::set_animation_frame(registry, entity, next_frame_index);
		}
	}
}
