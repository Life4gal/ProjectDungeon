// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/render.hpp>
#include <blueprint/detail/collision.hpp>

namespace pd::blueprint
{
	// 门
	class Door final
	{
	public:
		// 门的位置
		Position position;

		// 门碰撞体偏移
		Position door_offset;
		// 门碰撞体大小
		Size door_size;

		// 感应区偏移
		Position sensor_offset;
		// 感应区大小
		Size sensor_size;

		// 渲染(必须是动态精灵)
		// 初始为暂停状态
		// 正放 -> 开门
		// 倒放 -> 关门
		Sprite::Dynamic sprite;
	};
}
