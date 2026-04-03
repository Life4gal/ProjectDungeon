// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cstdint>

namespace pd::events::dungeon
{
	// 进入指定楼层的指定房间
	class Go final
	{
	public:
		// 层级
		std::uint32_t level;
		// 指定房间X
		std::uint32_t x;
		// 指定房间Y
		std::uint32_t y;
	};
}
