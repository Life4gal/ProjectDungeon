// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Vector2.hpp>

namespace pd::component::transform
{
	// 世界坐标
	// 无物理体: Position == 实体位置
	// 有物理体: Position == 物理体位置(移动时同步)
	class Position final
	{
	public:
		sf::Vector2f position;
	};

	// 屏幕坐标
	// 当且仅当目标实体在屏幕(相机?)区域才存在该组件
	// class ScreenPosition final
	// {
	// public:
	// 	sf::Vector2i position;
	// };

	// class [[deprecated("这个组件应该用不到了")]] Scale final
	// {
	// public:
	// 	sf::Vector2f scale;
	// };

	// 旋转
	// 无物理体: Rotation == 实体旋转
	// 有物理体: Rotation == 物理体旋转(旋转时同步)
	class Rotation final
	{
	public:
		sf::Angle rotation;
	};
}
