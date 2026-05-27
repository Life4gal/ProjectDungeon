// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <box2d/id.h>

namespace pd::component::collision
{
	// 碰撞体的刚体ID
	class BodyId final
	{
	public:
		b2BodyId body_id;
	};

	// 碰撞体的形状ID
	// 其实也可以不保存
	class ShapeIds final
	{
	public:
		std::vector<b2ShapeId> shape_ids;
	};
}
