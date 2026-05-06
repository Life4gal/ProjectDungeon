// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Vector2.hpp>

namespace pd::component::camera
{
	// 相机区域位置/大小有变化,需要更新视野内实体
	class Dirty final {};

	// 相机区域位置
	class Position final
	{
	public:
		sf::Vector2f position;
	};

	// 相机区域大小
	// NOTE: 相机区域大小一般*不*等于屏幕大小,理论上相机区域仅表示*游戏区域*大小,不包含HUD区域大小(如果我们有的话)
	class Size final
	{
	public:
		sf::Vector2f size;
	};
}
