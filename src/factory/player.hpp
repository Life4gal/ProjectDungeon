// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

namespace pd::factory
{
	// 在指定位置生成玩家
	auto player(entt::registry& registry, sf::Vector2f position, sf::Vector2f scale = sf::Vector2f{1, 1}, sf::Angle rotation = sf::degrees(0)) noexcept -> entt::entity;
}
