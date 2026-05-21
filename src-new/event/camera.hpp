// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace pd::event::camera
{
	// 相机区域被设置
	class SetArea final
	{
	public:
		sf::FloatRect previous;
		sf::FloatRect current;
	};

	// 相机被移动
	class Move final
	{
	public:
		sf::Vector2f previous;
		sf::Vector2f current;
	};

	// 相机被平移
	class Translate final
	{
	public:
		sf::Vector2f previous;
		sf::Vector2f current;
		sf::Vector2f distance;
	};

	// 相机区域被改变
	class Resize final
	{
	public:
		sf::Vector2f previous;
		sf::Vector2f current;
	};
}
