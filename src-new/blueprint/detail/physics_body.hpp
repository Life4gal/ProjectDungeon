// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cstdint>

namespace pd::blueprint
{
	enum class PhysicsBodyType : std::uint8_t
	{
		// b2_staticBody
		STATIC = 0,

		// b2_kinematicBody
		KINEMATIC,

		// b2_dynamicBody
		DYNAMIC,
	};

	class PhysicsBody final
	{
	public:
		PhysicsBodyType type;

		bool is_bullet;

		// TODO: 其他需要的信息?
	};
}
