// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/position.hpp>
#include <blueprint/detail/sprite.hpp>
#include <blueprint/detail/physics_body.hpp>
#include <blueprint/detail/physics_shape.hpp>

namespace pd::blueprint
{
	class Wall final
	{
	public:
		Position position;
		Sprite sprite;

		// 物理刚体
		PhysicsBody physics_body;
		// 矩形碰撞体
		PhysicsShapeBox physics_shape;
	};
}
