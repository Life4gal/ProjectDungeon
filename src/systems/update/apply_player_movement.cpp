// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/update/apply_player_movement.hpp>

#include <systems/helper/player.hpp>

namespace pd::systems::update
{
	auto apply_player_movement(entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		using namespace systems;

		// 应用由Player::handler_event设置的玩家移动状态
		helper::Player::apply_movement(registry);
	}
}
