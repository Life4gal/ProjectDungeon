// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/limited_life.hpp>

#include <component/limited_life.hpp>
#include <component/tags.hpp>

#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;

	namespace
	{
		auto time(entt::registry& registry, const sf::Time delta) noexcept -> void
		{
			namespace llt = limited_life::time;

			for (const auto view = registry.view<const llt::Lifetime, llt::Elapsed>();
			     const auto [entity, lifetime, elapsed]: view.each())
			{
				elapsed.elapsed += delta;
				if (elapsed.elapsed >= lifetime.lifetime)
				{
					// 标记为可被销毁
					registry.emplace<state::EntityDead>(entity);
				}
			}
		}
	}

	auto limited_life(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		// ==================================
		// TIME
		// ==================================

		time(registry, delta);

		// ==================================
		// 
		// ==================================
	}
}
