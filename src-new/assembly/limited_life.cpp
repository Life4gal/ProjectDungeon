// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <assembly/limited_life.hpp>

#include <component/limited_life.hpp>

#include <entt/entt.hpp>

namespace pd::assembly
{
	using namespace component;

	auto LimitedLife::make(entt::registry& registry, const entt::entity entity, const blueprint::LimitedLife::Time& time) noexcept -> void
	{
		registry.emplace<limited_life::time::Lifetime>(entity, sf::milliseconds(time.lifetime_ms));
		registry.emplace<limited_life::time::Elapsed>(entity, sf::Time::Zero);
	}

	auto LimitedLife::make(entt::registry& registry, const entt::entity entity, const blueprint::LimitedLife& limited_life) noexcept -> void
	{
		std::visit(
			[&](const auto& ll) noexcept -> void
			{
				make(registry, entity, ll);
			},
			limited_life.limited_life
		);
	}
}
