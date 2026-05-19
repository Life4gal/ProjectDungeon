// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/sprite_animation.hpp>
#include <blueprint/detail/physics.hpp>
#include <blueprint/detail/actor.hpp>

namespace pd::blueprint
{
	class Player final
	{
	public:
		// 精灵动画
		SpriteAnimation animation;

		// 生成位置
		Position position;

		// Actor
		Actor actor;

		// 圆形物理体
		BodyDesc body_desc;
		ShapeDesc shape_desc;
		ShapeCategory::Circle shape;
	};
}
