// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

namespace pd::component::transform
{
	// 世界位置
	class Position final
	{
	public:
		sf::Vector2f position;
	};

	// 缩放
	class Scale final
	{
	public:
		sf::Vector2f scale;
	};

	// 旋转
	class Rotation final
	{
	public:
		sf::Angle rotation;
	};
}
