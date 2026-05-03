// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cstdint>

namespace pd::blueprint
{
	enum class MoveBehavior : std::uint8_t
	{
		// 不移动
		STATIONARY = 0,
		// 随机移动
		WANDER,
		// 跳跃移动
		JUMP,
		// 追逐玩家
		CHASE,
		// 传送
		TELEPORT,
	};

	class Ai final
	{
	public:
		MoveBehavior move_behavior;
	};
}
