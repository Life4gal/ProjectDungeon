// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Time.hpp>

namespace pd::component::ai
{
	// =========================================
	// WANDER
	// =========================================

	namespace wander
	{
		// 当前移动方向
		class Direction final
		{
		public:
			sf::Angle angle;
		};

		// 朝当前移动方向继续移动的剩余时间
		class DirectionTimer final
		{
		public:
			sf::Time remaining;
		};
	}

	// =========================================
	// JUMP
	// =========================================

	namespace jump
	{
		// 当前状态
		enum class State : std::uint8_t
		{
			IDLE,
			JUMPING
		};

		// 滞空计时器
		class AirTimer final
		{
		public:
			sf::Time remaining;
		};
	}

	// =========================================
	// CHASE
	// =========================================

	namespace chase
	{
		class Placeholder final {};
	}

	// =========================================
	// 
	// =========================================
}
