// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <array>

#include <blueprint/room.hpp>

namespace pd::designer
{
	class Room final
	{
	public:
		using size_type = blueprint::LayoutPosition::size_type;

		constexpr static int tile_width = 64;
		constexpr static int tile_height = 64;
		constexpr static int tile_origin_x = tile_width / 2;
		constexpr static int tile_origin_y = tile_height / 2;

		// 房间横向瓦片的数量
		constexpr static size_type horizontal_count = 33;
		// 房间纵向瓦片的数量
		constexpr static size_type vertical_count = 21;
		// 房间宽度
		constexpr static size_type width = horizontal_count * tile_width;
		// 房间高度
		constexpr static size_type height = vertical_count * tile_height;
		// 房间位置 = (offset_x * width, offset_y * height)

		// 生成一个标准房间
		[[nodiscard]] static auto standard(size_type offset_x, size_type offset_y, blueprint::DirectionMask neighbors) noexcept -> blueprint::Room;
	};

	class RoomNavigation final
	{
	public:
		enum class DirectionValue : std::make_signed_t<Room::size_type>
		{
			BACKWARD = -1,
			NONE = 0,
			FORWARD = 1,
		};

		class Direction final
		{
		public:
			using value_type = std::underlying_type_t<DirectionValue>;

			value_type x;
			value_type y;
		};

		// 各个方向的坐标偏移
		constexpr static std::array<Direction, 4> directions
		{{
				// NORTH
				{.x = std::to_underlying(DirectionValue::NONE), .y = std::to_underlying(DirectionValue::BACKWARD)},
				// SOUTH
				{.x = std::to_underlying(DirectionValue::NONE), .y = std::to_underlying(DirectionValue::FORWARD)},
				// WEST
				{.x = std::to_underlying(DirectionValue::BACKWARD), .y = std::to_underlying(DirectionValue::NONE)},
				// EAST
				{.x = std::to_underlying(DirectionValue::FORWARD), .y = std::to_underlying(DirectionValue::NONE)}
		}};

		[[nodiscard]] constexpr static auto direction_of(const blueprint::Direction neighbor) noexcept -> Direction
		{
			if (neighbor == blueprint::Direction::NORTH)
			{
				return directions[0];
			}
			if (neighbor == blueprint::Direction::SOUTH)
			{
				return directions[1];
			}
			if (neighbor == blueprint::Direction::WEST)
			{
				return directions[2];
			}
			if (neighbor == blueprint::Direction::EAST)
			{
				return directions[3];
			}

			return {.x = std::to_underlying(DirectionValue::NONE), .y = std::to_underlying(DirectionValue::NONE)};
		}
	};
}
