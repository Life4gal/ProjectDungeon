// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/initialize/debug.hpp>

#include <systems/helper/player.hpp>

namespace pd::systems::initialize
{
	auto debug(entt::registry& registry) noexcept -> void
	{
		// 创建一个测试用实体
		// 64 / 16 == 4
		helper::Player::spawn(registry, sf::Vector2f{500, 500}, sf::Vector2f{4, 4});
	}
} // namespace pd::systems::initialize
