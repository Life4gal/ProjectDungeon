// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/room.hpp>

namespace pd::designer
{
	enum class RoomNeighbor : std::uint8_t
	{
		NONE = 0,

		NORTH = 0b0001,
		SOUTH = 0b0010,
		WEST = 0b0100,
		EAST = 0b1000
	};

	class Room final
	{
	public:
		using size_type = std::uint32_t;

		constexpr static int tile_width = 64;
		constexpr static int tile_height = 64;
		constexpr static int tile_origin_x = tile_width / 2;
		constexpr static int tile_origin_y = tile_height / 2;

		// 房间横向瓦片的数量
		constexpr static size_type horizontal_count = 17;
		// 房间纵向瓦片的数量
		constexpr static size_type vertical_count = 11;
		// 房间宽度
		constexpr static size_type width = horizontal_count * tile_width;
		// 房间高度
		constexpr static size_type height = vertical_count * tile_height;
		// 房间位置 = (offset_x * width, offset_y * height)

		// 生成一个标准房间
		[[nodiscard]] static auto standard(size_type offset_x, size_type offset_y, std::underlying_type_t<RoomNeighbor> neighbors) noexcept -> blueprint::Room;
	};
}
