// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/room.hpp>

#include <algorithm>
// std::popcount
#include <bit>

#include <designer/floor.hpp>
#include <designer/wall.hpp>
#include <designer/door.hpp>
#include <designer/enemy.hpp>

#include <prometheus/platform/os.hpp>

namespace pd::designer
{
	auto Room::standard(const size_type offset_x, const size_type offset_y, const std::underlying_type_t<blueprint::RoomConnection> neighbors) noexcept -> blueprint::Room
	{
		const auto neighbors_count = std::popcount(neighbors);

		// ===========================================
		// FLOOR
		// ===========================================

		auto floors = [] noexcept -> std::vector<blueprint::Floor>
		{
			std::vector<blueprint::Floor> fs{};
			fs.reserve(static_cast<std::size_t>(horizontal_count) * vertical_count);

			for (size_type y = 1; y < vertical_count - 1; ++y)
			{
				for (size_type x = 1; x < horizontal_count - 1; ++x)
				{
					fs.push_back(Floor::standard(x, y));
				}
			}

			return fs;
		}();

		// ===========================================
		// WALL
		// ===========================================

		auto walls = [&] noexcept -> std::vector<blueprint::Wall>
		{
			std::vector<blueprint::Wall> ws{};
			ws.reserve(horizontal_count * 2 + (vertical_count - 2) * 2);

			// 上面/下面
			for (const size_type y: {size_type{0}, vertical_count - 1})
			{
				for (size_type x = 0; x < horizontal_count; ++x)
				{
					// 跳过中间那一格,那是门所在
					if (x == horizontal_count / 2)
					{
						if (y == 0 and (neighbors & std::to_underlying(blueprint::RoomConnection::NORTH)))
						{
							continue;
						}

						if (y == vertical_count - 1 and (neighbors & std::to_underlying(blueprint::RoomConnection::SOUTH)))
						{
							continue;
						}
					}

					ws.push_back(Wall::standard(x, y));
				}
			}
			// 左面/右面
			for (const size_type x: {size_type{0}, horizontal_count - 1})
			{
				for (size_type y = 1; y < vertical_count - 1; ++y)
				{
					// 跳过中间那一格,那是门所在
					if (y == vertical_count / 2)
					{
						if (x == 0 and (neighbors & std::to_underlying(blueprint::RoomConnection::WEST)))
						{
							continue;
						}

						if (x == horizontal_count - 1 and (neighbors & std::to_underlying(blueprint::RoomConnection::EAST)))
						{
							continue;
						}
					}

					ws.push_back(Wall::standard(x, y));
				}
			}

			return ws;
		}();

		// ===========================================
		// DOOR
		// ===========================================

		auto doors = [&] noexcept -> std::vector<blueprint::Door>
		{
			std::vector<blueprint::Door> ds;
			ds.reserve(neighbors_count);

			if (neighbors & std::to_underlying(blueprint::RoomConnection::NORTH))
			{
				ds.push_back(Door::standard(horizontal_count / 2, 0, blueprint::DoorDirection::NORTH));
			}
			if (neighbors & std::to_underlying(blueprint::RoomConnection::SOUTH))
			{
				ds.push_back(Door::standard(horizontal_count / 2, vertical_count - 1, blueprint::DoorDirection::SOUTH));
			}
			if (neighbors & std::to_underlying(blueprint::RoomConnection::WEST))
			{
				ds.push_back(Door::standard(0, vertical_count / 2, blueprint::DoorDirection::WEST));
			}
			if (neighbors & std::to_underlying(blueprint::RoomConnection::EAST))
			{
				ds.push_back(Door::standard(horizontal_count - 1, vertical_count / 2, blueprint::DoorDirection::EAST));
			}

			return ds;
		}();

		// ===========================================
		// ENEMY
		// ===========================================

		auto enemies = [&] noexcept -> std::vector<blueprint::Enemy>
		{
			std::vector<blueprint::Enemy> es{};
			es.reserve(3);

			es.push_back(Enemy::rat(2, 2));
			es.push_back(Enemy::slime(3, 3));
			es.push_back(Enemy::bat(4, 4));

			return es;
		}();

		// ===========================================
		// ROOM
		// ===========================================

		const auto room_x = static_cast<float>(offset_x * width);
		const auto room_y = static_cast<float>(offset_y * height);

		const auto process_offset = [room_x, room_y](blueprint::Position& position) noexcept -> void
		{
			position.x += room_x;
			position.y += room_y;
		};

		std::ranges::for_each(floors, process_offset, &blueprint::Floor::position);
		std::ranges::for_each(walls, process_offset, &blueprint::Wall::position);
		std::ranges::for_each(doors, process_offset, &blueprint::Door::position);
		std::ranges::for_each(enemies, process_offset, &blueprint::Enemy::position);

		return
		{
				.type = blueprint::RoomType::STANDARD,
				.connection = neighbors,
				.position = {.x = room_x, .y = room_y},
				.size = {.width = width, .height = height},
				.floors = std::move(floors),
				.walls = std::move(walls),
				.doors = std::move(doors),
				.enemies = std::move(enemies)
		};
	}
}
