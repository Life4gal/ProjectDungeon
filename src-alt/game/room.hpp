// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <array>

namespace pd::game
{
	enum class DoorDirection : std::uint8_t
	{
		NORTH = 0,
		SOUTH,
		WEST,
		EAST,

		COUNT
	};

	class Room final
	{
	public:
		using size_type = std::uint32_t;

		class Position
		{
		public:
			size_type x;
			size_type y;
		};

		constexpr static auto grid_width = 15;
		constexpr static auto grid_height = 9;
		constexpr static auto door_count = 4;

		constexpr static std::array<Position, door_count> door_positions
		{{
				// NORTH
				{grid_width / 2, 0},
				// SOUTH
				{grid_width / 2, grid_height - 1},
				// WEST
				{0, grid_height / 2},
				// EAST
				{grid_width - 1, grid_height / 2}
		}};

		[[nodiscard]] constexpr static auto position_of(const DoorDirection direction) noexcept -> Position
		{
			return door_positions[std::to_underlying(direction)];
		}
	};
}
