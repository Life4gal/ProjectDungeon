// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

namespace pd::config
{
	class Tile;
}

namespace pd::systems::helper
{
	class Floor final
	{
	public:
		// 使用瓦片配置在指定位置生成地板
		// 地板没有物理碰撞体,只用于渲染
		static auto spawn(
			entt::registry& registry,
			const config::Tile& tile,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1},
			sf::Angle rotation = sf::degrees(0)
		) noexcept -> entt::entity;

		// 使用瓦片配置在指定位置生成地板洞
		// 地板洞有物理碰撞体,阻止地面移动,但是可以飞过
		static auto spawn_hole(
			entt::registry& registry,
			const config::Tile& tile,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1},
			sf::Angle rotation = sf::degrees(0)
		) noexcept -> entt::entity;
	};
}
