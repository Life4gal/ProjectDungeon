// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <array>

#include <blueprint/tile.hpp>
#include <blueprint/enemy.hpp>
#include <blueprint/npc.hpp>

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

		// 门感应区
		// 基于neighbors创建
		// 不使用Collision,因为其BODY/SHAPE相关定义是确定的(一个小小的优化)
		class DoorSensor final
		{
		public:
			// STATIC BODY
			// DEFAULT SHAPE DEF (SENSOR)
			// BOX SHAPE
			CollisionShape::box shape;
			// 感应器位置
			Position position;
		};

		std::array<DoorSensor, 4> door_sensors;

		// 房间边界(墙壁碰撞体)
		// 不再给每个墙壁一个碰撞体,而是直接使用线段来实现墙壁的作用
		// 如果需要,这甚至可以实现下面的*tiles*的大部分collision的功能[阻止移动 :)]
		// 不使用Collision,因为其BODY/SHAPE相关定义是确定的(一个小小的优化)
		class Bounding final
		{
		public:
			// STATIC BODY
			// DEFAULT SHAPE DEF (SOLID)
			// SEGMENT SHAPE
			CollisionShape::segment shape;

			// position = 房间位置
		};

		// 我们其实可以"算"出来如何设置房间边界
		// 不过直接定义也没什么问题
		// 以房间坐标为(0,0),房间大小为(90,90),房间内瓦片大小为10X10,房间邻居只有右+下为例:
		// bounding = 
		// {
		//  // 左上到右上
		//  { {10,10}, {80,0} },
		//  // 左上到左下
		//  { {10, 10}, {10,80} },
		//  // 右上到右下,中间有间隔(门)(别忘了门区域也需要限制,否则可能从该侧进入墙壁区域)
		//  { {80,10}, {80,40} }, { {80,40}, {90,40} }, { {80,50}, {80,80} },
		//  左下到右下,中间有间隔(门)(别忘了门区域也需要限制,否则可能从该侧进入墙壁区域)
		//  { {10,80}, {40,80} }, { {40,80}, {40,90} }, { {50,80}, {80,80} },
		// }
		std::vector<Bounding> bounding;

		// 房间内所有瓦片
		std::vector<Tile> tiles;

		// 房间内所有敌人
		// TODO: 理论上敌人应该是独立于房间的存在?
		std::vector<Enemy> enemies;

		// 房间内所有NPC
		std::vector<Npc> npc;
	};
}
