// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cstdint>

namespace pd::bp
{
	enum class Direction : std::uint8_t
	{
		NORTH = 0b00,
		SOUTH = 0b01,
		WEST = 0b10,
		EAST = 0b11,
	};

	enum class DirectionMask : std::uint8_t
	{
		NONE = 0,

		NORTH = 0b0001,
		SOUTH = 0b0010,
		WEST = 0b0100,
		EAST = 0b1000
	};

	// [[nodiscard]] constexpr auto operator-(const Direction direction) noexcept -> Direction
	// {
	// 	return static_cast<Direction>(std::to_underlying(direction) ^ 0b01);
	// }
}
