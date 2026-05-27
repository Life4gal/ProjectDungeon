// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Angle.hpp>
#include <SFML/System/Time.hpp>

namespace pd::component::move_behavior
{
	// =========================================
	// WANDER
	// =========================================

	namespace wander
	{
		// 蓝图配置
		class Config final
		{
		public:
			// 移动速度
			float speed;
			// 考虑下一次转向的最小时间
			float next_turn_min_time;
			// 考虑下一次转向的最大时间
			float next_turn_max_timer;
			//
		};

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
	// PATROL
	// =========================================

	namespace patrol
	{
		// TODO
	}

	// =========================================
	// CHASE
	// =========================================

	namespace chase
	{
		// 蓝图配置
		class Config final
		{
		public:
			// 移动速度
			float speed;
		};
	}

	// =========================================
	// JUMP
	// =========================================

	namespace jump
	{
		// TODO
	}

	// =========================================
	// CHASE JUMP
	// =========================================

	namespace chase_jump
	{
		// 蓝图配置
		class Config final
		{
		public:
			// 跳跃速度
			float speed;
			// 跳跃持续时间
			float duration;
			// 考虑下一次跳跃的最小时间
			float next_jump_min_time;
			// 考虑下一次跳跃的最大时间
			float next_jump_max_time;
		};

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
	// TELEPORT
	// =========================================

	namespace teleport
	{
		// TODO
	}

	// =========================================
	// 
	// =========================================
}
