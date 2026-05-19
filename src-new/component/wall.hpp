// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/transform.hpp>
#include <component/sprite.hpp>
#include <component/physics.hpp>
#include <component/tags.hpp>

namespace pd::component::wall
{
	// transform --> 墙壁变换
	// sprite --> 墙壁精灵
	// physics --> 墙壁物理体
	// tags --> 墙壁标签

	// 物理碰撞体
	class PhysicsShape final
	{
	public:
		b2ShapeId shape;
	};
}
