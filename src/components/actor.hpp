// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

// #include <SFML/System/Vector2.hpp>

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
	// todo: 这里的速度是当前最大速度?(即可能被修正的最大速度),还是正常情况下(无干扰)的最大速度?
	class Speed final
	{
	public:
		// 速度
		float speed;
		// 速度的平方(避免每次都计算)
		float speed_squared;
	};

	// 我们当前通过b2World_GetBodyEvents通过实体的transform信息,我们需要下面这些组件吗?

	// // 当前速度
	// class Velocity final
	// {
	// public:
	// 	sf::Vector2f value;
	// };
	//
	// // 当前速度方向(已归一化)
	// class Direction final
	// {
	// public:
	// 	sf::Vector2f direction;
	// };
} // namespace pd::components::actor
