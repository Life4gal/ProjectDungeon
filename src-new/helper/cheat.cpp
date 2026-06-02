// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/cheat.hpp>

#include <component/enemy.hpp>

#include <helper/property.hpp>

#include <entt/entt.hpp>

namespace pd::helper
{
	using namespace component;

	auto Cheat::kill_all_enemy(entt::registry& registry) noexcept -> void
	{
		for (const auto view = registry.view<tags::Enemy, state::InCameraArea>();
		     const auto [entity]: view.each())
		{
			Property::kill(registry, entity, entt::null);
		}
	}

	auto Cheat::set_all_enemy_hp_percent(entt::registry& registry, const float percent) noexcept -> void
	{
		for (const auto view = registry.view<tags::Enemy, state::InCameraArea>();
		     const auto [entity]: view.each())
		{
			if (const auto max_health = Property::get_max_health(registry, entity);
				max_health >= 0)
			{
				const auto health = max_health * percent;

				Property::set_health(registry, entity, health);
			}
		}
	}
}
