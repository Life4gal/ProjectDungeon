// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/graveyard.hpp>

#include <component/state.hpp>

#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;

	auto graveyard(entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		const auto view = registry.view<state::Dead>();
		registry.destroy(view.begin(), view.end());
	}
}
