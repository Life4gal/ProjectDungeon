// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/initialize/debug.hpp>

#include <components/transform.hpp>
#include <components/animation.hpp>
#include <components/render.hpp>

#include <ctx/animation.hpp>

#include <entt/entt.hpp>

namespace pd::systems::initialize
{
	auto debug(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		// 创建一个测试用实体
		const auto entity = registry.create();

		// transform
		{
			using namespace transform;

			registry.emplace<Position>(entity, sf::Vector2f{500, 500});
			registry.emplace<Scale>(entity, sf::Vector2f{10, 10});
			registry.emplace<Rotation>(entity, sf::degrees(0));
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
	}
} // namespace pd::systems::initialize
