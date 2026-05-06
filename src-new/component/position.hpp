// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Vector2.hpp>

namespace pd::component::position
{
	// 世界坐标
	class World final
	{
	public:
		sf::Vector2f position;
	};

	// // 屏幕坐标
	// // 当且仅当目标实体在屏幕(相机?)区域才存在该组件
	// class Screen final
	// {
	// public:
	// 	sf::Vector2i position;
	// };
}
