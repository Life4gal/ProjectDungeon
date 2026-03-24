// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

namespace pd::components::transform
{
	class Position
	{
	public:
		sf::Vector2f position;
	};

	class Scale
	{
	public:
		sf::Vector2f scale;
	};

	class Rotation
	{
	public:
		sf::Angle rotation;
	};
}
