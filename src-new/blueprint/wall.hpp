// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/sprite.hpp>
#include <blueprint/detail/physics.hpp>

namespace pd::blueprint
{
	class Wall final
	{
	public:
		// 精灵
		Sprite sprite;

		// 生成位置
		Position position;

		// 矩形物理体
		BodyDesc body_desc;
		ShapeDesc shape_desc;
		ShapeCategory::Box shape;
	};
}
