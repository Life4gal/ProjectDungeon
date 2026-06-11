// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/dynamic_sprite.hpp>

#include <component/render.hpp>

#include <accessor/dynamic_sprite.hpp>
#include <helper/dynamic_sprite.hpp>

#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;

	auto dynamic_sprite(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		namespace rss = render::static_sprite;
		namespace rds = render::dynamic_sprite;

		const auto view = registry
				.view<
					const rds::Frames,
					const rds::BeginFrameIndex,
					const rds::EndFrameIndex,
					rds::CurrentFrameIndex,
					rds::CurrentFrameElapsed,
					const rds::AnimationMode,
					const rds::AnimationDirection>(
					entt::exclude<
						// 如果动画暂停则无需更新动画
						rds::Paused,
						// 如果动画已经结束(例如非循环动画播完了最后一帧)则无需更新动画
						rds::Ended
					>
				);

		for (const auto [entity, frames, begin_frame_index, end_frame_index, current_frame_index, current_frame_elapsed, mode, direction]: view.each())
		{
			const auto& this_frame = frames.frames[current_frame_index.index];

			current_frame_elapsed.elapsed += delta;
			// 如果此帧未结束,无需更新
			if (current_frame_elapsed.elapsed < this_frame.duration)
			{
				continue;
			}

			// 帧计时并不重置为0,而是减去当前帧的持续时间
			current_frame_elapsed.elapsed -= this_frame.duration;

			// 跳转到下一帧
			if (const auto next_frame_index = helper::DynamicSprite::jump_to_next_frame(begin_frame_index, end_frame_index, current_frame_index, mode, direction);
				next_frame_index == accessor::DynamicSprite::animation_ended)
			{
				// 如果动画已结束则标记为已结束
				accessor::DynamicSprite::end(registry, entity);
			}
			else
			{
				// 切换sprite
				const auto& [texture, position, size, pivot, duration] = frames.frames[current_frame_index.index];

				// 如果动画每帧间隔较长,而FPS较高时,每次都遍历Texture&Position&Size&Pivot会比较浪费性能
				// 在动画帧切换时才获取&更新这些组件
				registry.replace<rss::Texture>(entity, texture);
				registry.replace<rss::UvPosition>(entity, position);
				registry.replace<rss::UvSize>(entity, size);
				registry.replace<rss::Pivot>(entity, pivot);
			}
		}
	}
}
