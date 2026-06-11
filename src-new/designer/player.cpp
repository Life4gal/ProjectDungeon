// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/player.hpp>

#include <designer/character.hpp>

namespace pd::designer
{
	auto Player::test_character() noexcept -> blueprint::Player
	{
		return
		{
				.character = Character::player_default(),
				.speed = 60 * 4.0f,
		};
	}
}
