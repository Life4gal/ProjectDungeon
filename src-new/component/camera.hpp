// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Vector2.hpp>

namespace pd::component::camera
{
	class Position final
	{
	public:
		sf::Vector2f position;
	};

	class Size final
	{
	public:
		sf::Vector2f size;
	};
}
