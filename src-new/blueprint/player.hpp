// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/render.hpp>
#include <blueprint/detail/collision.hpp>
#include <blueprint/detail/property.hpp>

namespace pd::blueprint
{
	// 玩家
	class Player final
	{
	public:
		// 玩家的初始位置
		Position position;

		// 渲染(必须是动态精灵)
		DynamicSprite sprite;

		// 碰撞体
		Collision collision;

		// 玩家属性
		Property property;
		// 移动速度
		// FIXME(OPT): 将它移动到合适的地方
		float speed;
	};
}
