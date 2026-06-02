// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/projectile.hpp>

#include <component/projectile.hpp>

#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;

	auto projectile(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		// FIXME: 可以检测飞弹的位置,一旦飞出屏幕(相机)区域则立刻销毁
		const auto view = registry
				.view<
					tags::Projectile,
					projectile::Lifetime,
					transform::Position
				>();

		for (const auto [entity, lifetime, position]: view.each())
		{
			lifetime.remaining -= delta;
			if (lifetime.remaining <= sf::Time::Zero)
			{
				// 标记为可被销毁
				registry.emplace<state::EntityDead>(entity);
				continue;
			}

			// TODO: 有没有拖尾效果?
		}
	}
}
