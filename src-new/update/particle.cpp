// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/particle.hpp>

#include <component/particle.hpp>

#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;

	auto particle(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		const auto view = registry
				.view<
					const particle::Duration,
					particle::Elapsed
				>();

		for (const auto [entity, duration, elapsed]: view.each())
		{
			elapsed.elapsed += delta;

			if (elapsed.elapsed >= duration.duration)
			{
				// 标记为可被销毁
				registry.emplace<state::EntityDead>(entity);
			}
		}
	}
}
