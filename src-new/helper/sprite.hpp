// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

namespace sf
{
	class RenderWindow;
}

namespace pd::helper
{
	// TODO: 我们本可以将所有实体按RenderLayer分层,然后一次性将它们渲染
	//  但是如果要渲染同层级的实体存在重叠,我们无法保证它们之间的渲染顺序,而这会造成*闪烁*效果(即两个实体相邻两帧的渲染顺序不同)
	//  暂时默认方案,后续改进
	class Sprite final
	{
	public:
		static auto floor(entt::registry& registry, sf::RenderWindow& window) noexcept -> void;

		static auto wall(entt::registry& registry, sf::RenderWindow& window) noexcept -> void;

		static auto door(entt::registry& registry, sf::RenderWindow& window) noexcept -> void;

		static auto enemy(entt::registry& registry, sf::RenderWindow& window) noexcept -> void;

		static auto player(entt::registry& registry, sf::RenderWindow& window) noexcept -> void;

		static auto projectile(entt::registry& registry, sf::RenderWindow& window) noexcept -> void;
	};
}
