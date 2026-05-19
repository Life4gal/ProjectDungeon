// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/transform.hpp>
#include <component/sprite_animation.hpp>
#include <component/physics.hpp>
#include <component/tags.hpp>

namespace pd::component::player
{
	// transform --> 玩家变换
	// sprite_animation --> 玩家精灵动画
	// physics --> 玩家物理体
	// tags --> 玩家标签

	// 物理碰撞体
	class PhysicsShape final
	{
	public:
		b2ShapeId shape;
	};

	// TODO: 其他组件
}
