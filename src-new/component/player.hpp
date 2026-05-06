// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/position.hpp>
#include <component/sprite_animation.hpp>
#include <component/physics_body.hpp>
#include <component/tags.hpp>

namespace pd::component::player
{
	// position::World --> 玩家绝对位置
	// position::Screen --> 玩家屏幕位置
	// sprite_animation --> 玩家精灵动画
	// physics_body --> 玩家物理刚体
	// tags --> 玩家标签

	// 物理碰撞体
	class PhysicsShape final
	{
	public:
		b2ShapeId shape;
	};

	// TODO: 其他组件
}
