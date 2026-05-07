// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <render/player.hpp>

#include <helper/sprite.hpp>

namespace pd::render
{
	auto player(entt::registry& registry, sf::RenderWindow& window) noexcept -> void
	{
		helper::Sprite::player(registry, window);
	}
}
