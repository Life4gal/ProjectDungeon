// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cstdint>

namespace pd::components::tile
{
	// ==========================================
	// 非实体组件
	// ==========================================

	enum class CollisionLogical : std::uint8_t
	{
		// 地板
		FLOOR,
		// 墙壁
		WALL,
		// 障碍物
		OBSTACLE,
		// 洞
		HOLE,
		// 可摧毁物
		DESTROYABLE,
	};

	enum class Transform : std::uint8_t
	{
		// 原始朝向
		IDENTITY,
		// 水平翻转
		HORIZONTAL_FLIP,
		// 垂直翻转
		VERTICAL_FLIP,
		// 顺时针旋转90度
		CW_90,
		// 逆时针旋转90度
		CCW_90,
		// 顺时针旋转180度
		ROTATE_180,
		// 转置
		TRANSPOSE,
		// 反转置
		ANTI_TRANSPOSE
	};

	// 精灵图索引 -- 可视物
	enum class Visual : std::uint8_t {};

	// 精灵图索引 -- 逻辑物
	enum class Object : std::uint8_t {};

	// ==========================================
	// 上下文组件
	// ==========================================

	//

	// ==========================================
	// 实体组件
	// ==========================================

	// 上面的组件我们不附加在实体上,而是保存在房间的组件中
}
