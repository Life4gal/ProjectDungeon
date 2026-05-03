// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/transform.hpp>
#include <component/sprite_animation.hpp>
#include <component/physics_body.hpp>

namespace pd::component::enemy
{
	// transform
	// sprite_animation
	// physics_body

	class PhysicsShape final
	{
	public:
		b2ShapeId shape;
	};

	// blueprint::EnemyType
	enum class Type : std::uint8_t
	{
		RAT = 0,
		SLIME,
		BAT,
	};

	class Enemy final {};
}
