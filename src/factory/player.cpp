// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/player.hpp>

#include <components/transform.hpp>
#include <components/animation.hpp>
#include <components/render.hpp>
#include <components/actor.hpp>
#include <components/entity/player.hpp>

#include <ctx/animation.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	auto player(entt::registry& registry, sf::Vector2f position, sf::Vector2f scale, sf::Angle rotation) noexcept -> entt::entity
	{
		using namespace components;

		const auto entity = registry.create();

		// todo: 读取数据

		// transform
		{
			using namespace transform;

			registry.emplace<Position>(entity, position);
			registry.emplace<Scale>(entity, scale);
			registry.emplace<Rotation>(entity, rotation);
		}
		// animation
		{
			using namespace animation;

			const auto animation = ctx::Animation::get(registry, "AntleredRascal");
			const auto& begin_frame = animation->frames[0];

			registry.emplace<Timer>(entity, Timer{.duration = begin_frame.duration, .elapsed = sf::Time::Zero});
			registry.emplace<Index>(entity, Index{.total = animation->frames.size(), .current = 0});
			registry.emplace<Animation>(entity, animation);

			if (animation->looping)
			{
				registry.emplace<Looping>(entity);
			}
		}
		// render
		{
			using namespace render;

			const auto& index = registry.get<const animation::Index>(entity);
			const auto& [animation] = registry.get<const animation::Animation>(entity);

			const auto& frame = animation->frames[index.current];

			// 纹理
			registry.emplace<Texture>(
				entity,
				Texture
				{
						.texture_path = frame.texture_path,
						.frame_x = frame.frame_x,
						.frame_y = frame.frame_y,
						.frame_width = frame.frame_width,
						.frame_height = frame.frame_height
				}
			);
			// 颜色
			registry.emplace<Color>(entity, sf::Color::White);
		}
		// actor
		{
			registry.emplace<actor::Health>(entity, actor::Health{.current = 100, .max = 100});
			registry.emplace<actor::Mana>(entity, actor::Mana{.current = 20, .max = 20});
			registry.emplace<actor::Speed>(entity, actor::Speed{.speed = 100, .speed_squared = 100 * 100});

			registry.emplace<actor::Velocity>(entity, sf::Vector2f{0, 0});
			registry.emplace<actor::Direction>(entity, sf::Vector2f{0, 0});
		}
		// player
		{
			//
		}

		return entity;
	}
} // namespace pd::factory
