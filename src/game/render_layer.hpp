// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cstdint>

namespace pd
{
	// 渲染层级
	// 数值小的先渲染,换句话说数值大的将会覆盖数值小的(如果有重叠)
	// 这里的值设置地和EntityType一致,但是值具体是多少其实无所谓(只要大小关系正确即可)
	enum class RenderLayer : std::uint64_t
	{
		// BACKGROUND = 0,

		// [0~7]

		// 地板
		FLOOR = 0b0000'0001,

		// [8~15]

		// 墙壁
		WALL = 0b0000'0001'0000'0000,
		// 墙壁装饰物(叠加在墙壁上)
		WALL_DECORATION = 0b0000'0010'0000'0000,
		// 门
		DOOR = 0b0000'0100'0000'0000,

		// [16~23]

		// 地板装饰物(不影响移动, 感应器)
		FLOOR_DECORATION = 0b0000'0001'0000'0000'0000'0000,
		// 地板洞(阻止地面移动,但是可以飞过)
		FLOOR_HOLE = 0b0000'0010'0000'0000'0000'0000,
		// 矮地形(阻止地面移动,但是可以飞过)
		LOW_TERRAIN = 0b0000'0100'0000'0000'0000'0000,
		// 高地形(阻止地面移动,不能飞过)
		HIGH_TERRAIN = 0b0000'1000'0000'0000'0000'0000,

		// [24~31]

		// 敌人实体
		ENEMY = 0b0000'0001'0000'0000'0000'0000'0000'0000,
		// 玩家实体
		PLAYER = 0b0000'0010'0000'0000'0000'0000'0000'0000,

		// [32~39]

		// 近战武器
		WEAPON = 0b0000'0001'0000'0000'0000'0000'0000'0000'0000'0000,
		// 飞弹
		PROJECTILE = 0b0000'0010'0000'0000'0000'0000'0000'0000'0000'0000,

		// [40~47]

		// 特效
		EFFECT = 0b0000'0001'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000,

		// [48~55]

		// [56~63]

		// UI元素
		UI = 0b0000'0001'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000,
	};
} // namespace pd
