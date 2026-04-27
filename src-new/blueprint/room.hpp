// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <blueprint/floor.hpp>
#include <blueprint/wall.hpp>

namespace pd::blueprint
{
	class Room final
	{
	public:
		// 房间偏移
		// entity-position = room-offset + position
		float offset_x;
		float offset_y;

		// 房间 -- 地板
		std::vector<Floor> floors;
		// 房间 -- 墙壁
		std::vector<Wall> walls;

		//
	};
}
