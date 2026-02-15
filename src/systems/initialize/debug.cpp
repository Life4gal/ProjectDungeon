// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/initialize/debug.hpp>

#include <factory/player.hpp>

namespace pd::systems::initialize
{
	auto debug(entt::registry& registry) noexcept -> void
	{
		// 创建一个测试用实体
		factory::player(registry, sf::Vector2f{500, 500}, sf::Vector2f{10, 10});
	}
} // namespace pd::systems::initialize
