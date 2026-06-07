// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <array>

#include <blueprint/detail/layout.hpp>

#include <blueprint/door.hpp>
#include <blueprint/bounding.hpp>
#include <blueprint/tile.hpp>
#include <blueprint/enemy.hpp>
#include <blueprint/npc.hpp>

namespace pd::blueprint
{
	class Room final
	{
	public:
		// 房间类型
		RoomType type;
		// 房间邻居
		DirectionMask neighbors;

		// 房间位置(布局)
		LayoutPosition layout_position;
		// 房间位置(世界)
		Position position;
		// 房间大小
		Size size;

		// 门
		// 基于neighbors创建
		std::array<Door, 4> doors;

		// 房间边界(墙壁)
		Bounding bounding;

		// 房间内所有瓦片
		std::vector<Tile> tiles;

		// 房间内所有敌人
		// TODO: 理论上敌人应该是独立于房间的存在?
		std::vector<Enemy> enemies;

		// 房间内所有NPC
		std::vector<Npc> npc;
	};
}
