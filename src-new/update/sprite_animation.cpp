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
					const sprite_animation::Sprites,
					const sprite_animation::Total,
					sprite_animation::Timer,
					sprite_animation::Index,
					const sprite_animation::Mode,
					const sprite_animation::Direction,
					// FIXME: 如果动画每帧间隔较长,而FPS较高时,每次都遍历Texture/Position/Size/Origin其实会比较浪费性能,可以考虑在动画帧切换时才获取&更新这些组件
					sprite::Texture,
					sprite::Position,
					sprite::Size,
					sprite::Origin
				>(entt::exclude<
					// 如果动画暂停则无需更新动画
					sprite_animation::Paused,
					// 如果动画已经结束(例如非循环动画播完了最后一帧)则无需更新动画
					sprite_animation::Ended
				>);

		for (const auto [entity, sprites, total, timer, index, mode, direction, texture, position, size, origin]: view.each())
		{
			timer.elapsed += delta;
			// 如果此帧未结束,无需更新
			if (timer.elapsed < timer.duration)
			{
				continue;
			}

			// 帧计时并不重置为0,而是减去当前帧的持续时间
			timer.elapsed -= timer.duration;

			using helper::SpriteAnimation;

			// 跳转到下一帧
			if (const auto next_frame_index = SpriteAnimation::jump_to_next_frame(total, index, mode, direction);
				next_frame_index == SpriteAnimation::animation_ended)
			{
				// 如果动画已结束则标记为已结束
				SpriteAnimation::end(registry, entity);
			}
			else
			{
				// 切换sprite
				const auto& sprite = sprites.sprites[next_frame_index];

				texture.texture = sprite.texture;
				position.position = sprite.position;
				size.size = sprite.size;
				origin.origin = sprite.origin;
			}
		}
	}
}
