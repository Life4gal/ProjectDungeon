// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

namespace pd::components::player
{
	// transform
	// render
	// animation
	// physics_body
	// actor

	class Movement final
	{
	public:
		enum class Type : std::uint8_t
		{
			STAY,
			FORWARD,
			BACKWARD,
		};

		Type x;
		Type y;
	};
}
