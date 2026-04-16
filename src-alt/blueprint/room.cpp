// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <blueprint/room.hpp>

namespace pd::blueprint
{
	namespace
	{
		auto place_base_grid(Room::layout_type& layout) noexcept -> void
		{
			using size_type = Room::size_type;

			// 角落墙壁
			layout[0, 0].collision_logical = TileCollision::WALL;
			layout[0, game::RoomVerticalGrid - 1].collision_logical = TileCollision::WALL;
			layout[game::RoomHorizontalGrid - 1, 0].collision_logical = TileCollision::WALL;
			layout[game::RoomHorizontalGrid - 1, game::RoomVerticalGrid - 1].collision_logical = TileCollision::WALL;

			// 南北墙壁
			for (size_type x = 1; x < game::RoomHorizontalGrid - 1; ++x)
			{
				layout[x, 0].collision_logical = TileCollision::WALL;
				layout[x, game::RoomVerticalGrid - 1].collision_logical = TileCollision::WALL;
			}

			// 东西墙壁
			for (size_type y = 1; y < game::RoomVerticalGrid - 1; ++y)
			{
				layout[0, y].collision_logical = TileCollision::WALL;
				layout[game::RoomHorizontalGrid - 1, y].collision_logical = TileCollision::WALL;
			}

			// 地板
			for (size_type y = 1; y < game::RoomVerticalGrid - 1; ++y)
			{
				for (size_type x = 1; x < game::RoomHorizontalGrid - 1; ++x)
				{
					layout[x, y].collision_logical = TileCollision::FLOOR;
				}
			}
		}
	}

	auto Room::generate_standard() -> layout_type
	{
		layout_type layout{};

		// 墙壁+地板
		place_base_grid(layout);
		// todo

		return layout;
	}
}
