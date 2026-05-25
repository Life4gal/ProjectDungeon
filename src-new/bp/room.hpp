// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <array>

#include <bp/tile.hpp>
#include <bp/enemy.hpp>
#include <bp/npc.hpp>

#include <bp/detail/direction.hpp>

namespace pd::bp
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
		// 直接使用CollisionBoxShape而不是Collision(一个小小的优化)
		// STATIC BODY
		// DEFAULT SHAPE DEF (SENSOR)
		// BOX SHAPE
		std::array<CollisionBoxShape, 4> door_sensors;

		// 房间边界(墙壁碰撞体)
		// 不再给每个墙壁一个碰撞体,而是直接使用线段来实现墙壁的作用
		// 如果需要,这甚至可以实现下面的*tiles*的大部分collision的功能[阻止移动 :)]
		// 直接使用CollisionSegmentShape而不是Collision(一个小小的优化)
		// STATIC BODY
		// DEFAULT SHAPE DEF (SOLID)
		// SEGMENT SHAPE
		// 我们其实可以"算"出来如何设置房间边界
		// 不过直接定义也没什么问题
		// 以房间坐标为(0,0),房间大小为(90,90),房间内瓦片大小为10X10,房间邻居只有右+下为例:
		// bounding = 
		// {
		//  // 左上到右上
		//  { {0,0}, {90,0} },
		//  // 左上到左下
		//  { {0, 0}, {0,90} },
		//  // 右上到右下,中间有间隔(门)
		//  { {90,0}, {90,40} }, { {90,50}, {90,90} },
		//  左下到右下,中间有间隔(门)
		//  { {0,90}, {40,90} }, { {50,90}, {90,90} },
		// }
		std::vector<CollisionSegmentShape> bounding;

		// 房间内所有瓦片
		std::vector<Tile> tiles;

		// 房间内所有敌人
		// TODO: 理论上敌人应该是独立于房间的存在?
		std::vector<Enemy> enemies;

		// 房间内所有NPC
		std::vector<Npc> npc;
	};
}
