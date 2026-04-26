// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/transform.hpp>
#include <component/sprite.hpp>
#include <component/physics_body.hpp>

namespace pd::component::wall
{
	// transform
	// sprite
	// physics_body

	class PhysicsShape final
	{
	public:
		b2ShapeId shape;
	};

	class Wall final {};
}
