// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

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

		// 房间位置(布局)
		LayoutPosition layout_position;
		// 房间位置(世界)
		Position position;
		// 房间大小
		Size size;

		// 边界(墙壁)
		Bounding bounding;
		// 门
		std::vector<Door> doors;
		// 瓦片
		std::vector<Tile> tiles;

		// 敌人生成
		std::vector<Enemy> enemies;
		// NPC生成
		std::vector<Npc> npc;
	};
}
