// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/position.hpp>
#include <component/sprite.hpp>
#include <component/physics_body.hpp>
#include <component/tags.hpp>

namespace pd::component::wall
{
	// position::World --> 墙壁绝对位置
	// position::Screen --> 墙壁屏幕位置
	// sprite --> 墙壁精灵
	// physics_body --> 墙壁物理刚体
	// tags --> 墙壁标签

	// 物理碰撞体
	class PhysicsShape final
	{
	public:
		b2ShapeId shape;
	};
}
