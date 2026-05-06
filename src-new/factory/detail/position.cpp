// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/position.hpp>

#include <component/position.hpp>

#include <entt/entt.hpp>

namespace pd::factory::detail
{
	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::Position position) noexcept -> void
	{
		using namespace component;

		// 世界坐标
		registry.emplace<position::World>(entity, sf::Vector2f{position.x, position.y});
		// 屏幕坐标
		// registry.emplace<position::Screen>(entity, sf::Vector2f{0, 0});
	}
}
