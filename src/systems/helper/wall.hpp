// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>

namespace pd::config
{
	class Tile;
}

namespace pd::systems::helper
{
	class Wall final
	{
	public:
		// 使用瓦片配置在指定位置生成墙壁
		static auto spawn(
			entt::registry& registry,
			const config::Tile& tile,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1},
			sf::Angle rotation = sf::degrees(0)
		) noexcept -> entt::entity;
	};
}
