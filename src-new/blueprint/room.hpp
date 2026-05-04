// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <blueprint/floor.hpp>
#include <blueprint/wall.hpp>
#include <blueprint/door.hpp>
#include <blueprint/enemy.hpp>

namespace pd::blueprint
{
	// 房间类型
	enum class RoomType : std::uint8_t
	{
		// 起始房间
		START,
		// 标准房间
		STANDARD,
		// BOSS房间
		BOSS,
		// BOSS房间门钥匙房间
		KEY,
		// 商人房间
		MERCHANT,
		// 奖励房间
		BONUS,
		// 出口房间
		EXIT,
	};

	// 房间的连接状态(邻居)
	enum class RoomConnection : std::uint8_t
	{
		NONE = 0,

		NORTH = 0b0001,
		SOUTH = 0b0010,
		WEST = 0b0100,
		EAST = 0b1000
	};

	class Room final
	{
	public:
		RoomType type;
		std::underlying_type_t<RoomConnection> connection;

		// 房间偏移
		// entity-position = room-offset + position
		float offset_x;
		float offset_y;

		// 房间 -- 地板
		std::vector<Floor> floors;
		// 房间 -- 墙壁
		std::vector<Wall> walls;
		// 房间 -- 门
		std::vector<Door> doors;

		// 房间 -- 敌人
		// TODO: 理论上敌人应该是独立于房间的存在?
		std::vector<Enemy> enemies;

		//
	};
}
