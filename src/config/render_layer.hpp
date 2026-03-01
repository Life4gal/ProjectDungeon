// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cstdint>

namespace pd::config
{
	// 渲染层级
	// 数值小的先渲染,换句话说数值大的将会覆盖数值小的(如果有重叠)
	enum class RenderLayer : std::uint32_t
	{
		// [0~7]

		// 地板
		FLOOR = 0b0000'0001,
		// 墙壁
		WALL = 0b0000'0010,
		// 装饰物
		DECORATION = 0b0000'0100,
		// 触发器
		TRIGGER = 0b0000'1000,
		// 钥匙
		KEY = 0b0001'0000,
		// 门
		DOOR = 0b0010'0000,

		// [8~15]

		ENEMY_GROUND = 0b0000'0001'0000'0000,
		PLAYER_GROUND = 0b0000'0010'0000'0000,
		ENEMY_AERIAL = 0b0000'0100'0000'0000,
		PLAYER_AERIAL = 0b0000'1000'0000'0000,

		// [16~23]

		// 飞弹
		PROJECTILE = 0b0000'0001'0000'0000'0000'0000,

		// [24~31]

		// 特效
		EFFECT = 0b0000'0001'0000'0000'0000'0000'0000,
	};
}
