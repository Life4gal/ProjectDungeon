// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Vector2.hpp>

namespace pd::components::actor
{
	// 血量
	class Health final
	{
	public:
		float current;
		float max;
	};

	// 魔法值
	class Mana final
	{
	public:
		float current;
		float max;
	};

	// 速度
	class Speed final
	{
	public:
		// 速度
		float speed;
		// 速度的平方(避免每次都计算)
		float speed_squared;
	};

	// 当前速度
	class Velocity final
	{
	public:
		sf::Vector2f value;
	};

	// 当前速度方向(已归一化)
	class Direction final
	{
	public:
		sf::Vector2f direction;
	};
} // namespace pd::components::actor
