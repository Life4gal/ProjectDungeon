// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/transform.hpp>
#include <component/sprite.hpp>
#include <component/physics_body.hpp>

namespace pd::component::door
{
	// transform
	// sprite
	// physics_body

	class PhysicsShapeDoor final
	{
	public:
		b2ShapeId shape;
	};

	class PhysicsShapeSensor final
	{
	public:
		b2ShapeId shape;
	};

	class PhysicsShapeBlocker final
	{
	public:
		b2ShapeId shape;
	};

	class Door final {};
}
