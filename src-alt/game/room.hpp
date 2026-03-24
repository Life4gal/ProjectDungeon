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
		NORTH = 0b00,
		SOUTH = 0b01,
		WEST = 0b10,
		EAST = 0b11,
	};

	[[nodiscard]] constexpr auto operator-(const DoorDirection direction) noexcept -> DoorDirection
	{
		return static_cast<DoorDirection>(std::to_underlying(direction) ^ 0b01);
	}

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
		constexpr static auto tile_width = 64;
		constexpr static auto tile_height = 64;
		constexpr static auto room_width = grid_width * tile_width;
		constexpr static auto room_height = grid_height * tile_height;

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
