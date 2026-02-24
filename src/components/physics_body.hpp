// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <box2d/id.h>

namespace pd::components::physics_body
{
	// 所有碰撞体都有刚体id
	class BodyId final
	{
	public:
		b2BodyId id;
	};

	// todo: 是否需要保存ShapeId?还是在需要时通过BodyId查询当前所有shape?
} // namespace pd::components::physics_body
