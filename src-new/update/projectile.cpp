// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/projectile.hpp>

#include <component/projectile.hpp>
#include <component/state.hpp>

#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;

	auto projectile(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		const auto view = registry
				.view<
					state::InCameraArea,
					tags::Projectile,
					const projectile::Type,
					projectile::Lifetime,
					transform::Position
				>(entt::exclude<state::Dead>);

		for (const auto [entity, type, lifetime, position]: view.each())
		{
			lifetime.remaining -= delta;
			if (lifetime.remaining <= sf::Time::Zero)
			{
				registry.emplace<state::Dead>(entity);
				continue;
			}

			// TODO: 有没有拖尾效果?
		}
	}
}
