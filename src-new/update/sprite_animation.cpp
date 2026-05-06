// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/sprite_animation.hpp>

#include <component/sprite_animation.hpp>

#include <helper/sprite_animation.hpp>

#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;

	auto sprite_animation(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		const auto view = registry
				.view<
					const sprite_animation::Frames,
					const sprite_animation::FramesCount,
					const sprite_animation::Duration,
					sprite_animation::Timer,
					sprite_animation::Index,
					const sprite_animation::Mode,
					const sprite_animation::Direction>(
					entt::exclude<
						// 如果动画暂停则无需更新动画
						sprite_animation::Paused,
						// 如果动画已经结束(例如非循环动画播完了最后一帧)则无需更新动画
						sprite_animation::Ended
					>
				);

		for (const auto [entity, frames, frames_count, duration, timer, index, mode, direction]: view.each())
		{
			timer.elapsed += delta;
			// 如果此帧未结束,无需更新
			if (timer.elapsed < duration.duration)
			{
				continue;
			}

			// 帧计时并不重置为0,而是减去当前帧的持续时间
			timer.elapsed -= duration.duration;

			using helper::SpriteAnimation;

			// 跳转到下一帧
			if (const auto next_frame_index = SpriteAnimation::jump_to_next_frame(frames_count, index, mode, direction);
				next_frame_index == SpriteAnimation::animation_ended)
			{
				// 如果动画已结束则标记为已结束
				SpriteAnimation::end(registry, entity);
			}
			else
			{
				// 切换sprite
				const auto& [texture, position] = frames.frames[next_frame_index];

				// 如果动画每帧间隔较长,而FPS较高时,每次都遍历Texture&TexturePosition会比较浪费性能
				// 在动画帧切换时才获取&更新这些组件
				// Size&Origin无需更新,因为SpriteAnimation要求所有帧必须相同
				registry.replace<sprite::Texture>(entity, texture);
				registry.replace<sprite::TexturePosition>(entity, position);
			}
		}
	}
}
