// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/room.hpp>

#include <algorithm>
// std::popcount
#include <bit>

#include <manager/random.hpp>

#include <designer/door.hpp>
#include <designer/tile.hpp>
#include <designer/enemy.hpp>

#include <prometheus/platform/os.hpp>

namespace pd::designer
{
	namespace
	{
		// 瓦片坐标 --> 瓦片左上角相对偏移(相对于房间位置的偏移)
		[[nodiscard]] constexpr auto anchor_of(const Room::size_type x, const Room::size_type y) noexcept -> blueprint::Offset
		{
			return
			{
					.x = static_cast<float>(x * Room::tile_width),
					.y = static_cast<float>(y * Room::tile_height),
			};
		}

		// 瓦片坐标 --> 瓦片中心点相对偏移(相对于房间位置的偏移)
		[[nodiscard]] constexpr auto center_of(const Room::size_type x, const Room::size_type y) noexcept -> blueprint::Offset
		{
			const auto [ax, ay] = anchor_of(x, y);

			return
			{
					.x = static_cast<float>(Room::tile_origin_x) + ax,
					.y = static_cast<float>(Room::tile_origin_y) + ay,
			};
		}

		// 瓦片坐标 --> 瓦片中心点相对偏移(相对于房间位置的偏移)(但是返回位置类型)
		[[nodiscard]] constexpr auto center_position_of(const Room::size_type x, const Room::size_type y) noexcept -> blueprint::Position
		{
			const auto [ox, oy] = center_of(x, y);
			return {.x = ox, .y = oy};
		}
	}

	auto Room::standard(const size_type offset_x, const size_type offset_y, const blueprint::DirectionMask neighbors) noexcept -> blueprint::Room
	{
		// ===========================
		// 类型
		// ===========================

		constexpr auto type = blueprint::RoomType::STANDARD;

		// ===========================
		// 位置 + 大小
		// ===========================

		const blueprint::LayoutPosition layout_position{.x = offset_x, .y = offset_y};
		const blueprint::Position position{.x = static_cast<float>(offset_x * width), .y = static_cast<float>(offset_y * height)};
		constexpr blueprint::Size size{.width = width, .height = height};

		// ===========================
		// 邻居
		// ===========================

		const auto neighbors_value = std::to_underlying(neighbors);
		const auto neighbors_count = std::popcount(neighbors_value);

		// 如果存在邻居,门所处瓦片坐标
		constexpr auto door_x = horizontal_count / 2;
		constexpr auto door_y = vertical_count / 2;

		// ===========================
		// 边界(墙壁)
		// ===========================

		blueprint::Bounding bounding{.position = {.x = 0, .y = 0}, .segments = {}};
		// 边界没有邻居 --> 一条线段
		// 边界有邻居 --> 五条线段
		bounding.segments.reserve(4 + 4 * neighbors_count);

		// 北面
		if (neighbors_value & std::to_underlying(blueprint::DirectionMask::NORTH))
		{
			// 边界
			//
			//         p3 >    < p4
			//                |门|
			//      -------     -------
			//     ^       ^   ^       ^
			//     p1     p2  p5     p6

			constexpr auto p1 = anchor_of(1, 1);
			constexpr auto p2 = anchor_of(door_x, 1);
			constexpr auto p3 = anchor_of(door_x, 0);
			constexpr auto p4 = anchor_of(door_x + 1, 0);
			constexpr auto p5 = anchor_of(door_x + 1, 1);
			constexpr auto p6 = anchor_of(horizontal_count - 1, 1);

			bounding.segments.emplace_back(p1, p2);
			bounding.segments.emplace_back(p2, p3);
			bounding.segments.emplace_back(p3, p4);
			bounding.segments.emplace_back(p4, p5);
			bounding.segments.emplace_back(p5, p6);
		}
		else
		{
			// 边界
			//
			//      -----------------
			//     ^                     ^
			//     p1                   p2

			constexpr auto p1 = anchor_of(1, 1);
			constexpr auto p2 = anchor_of(horizontal_count - 1, 1);

			bounding.segments.emplace_back(p1, p2);
		}
		// 南面
		if (neighbors_value & std::to_underlying(blueprint::DirectionMask::SOUTH))
		{
			// 边界
			//
			//      p1    p2  p5     p6
			//      v       v    v       v
			//      -------     -------
			//                |门|
			//        p3 >     < p4

			constexpr auto p1 = anchor_of(1, vertical_count - 1);
			constexpr auto p2 = anchor_of(door_x, vertical_count - 1);
			constexpr auto p3 = anchor_of(door_x, vertical_count);
			constexpr auto p4 = anchor_of(door_x + 1, vertical_count);
			constexpr auto p5 = anchor_of(door_x + 1, vertical_count - 1);
			constexpr auto p6 = anchor_of(horizontal_count - 1, vertical_count - 1);

			bounding.segments.emplace_back(p1, p2);
			bounding.segments.emplace_back(p2, p3);
			bounding.segments.emplace_back(p3, p4);
			bounding.segments.emplace_back(p4, p5);
			bounding.segments.emplace_back(p5, p6);
		}
		else
		{
			// 边界
			//
			//     p1                    p2
			//     v                       v
			//      -----------------

			constexpr auto p1 = anchor_of(1, vertical_count - 1);
			constexpr auto p2 = anchor_of(horizontal_count - 1, vertical_count - 1);

			bounding.segments.emplace_back(p1, p2);
		}
		// 西面
		if (neighbors_value & std::to_underlying(blueprint::DirectionMask::WEST))
		{
			// 边界
			//  
			//         < p1
			//         |
			//  p3   |
			//  v     |
			//   ---  < p2
			//   门
			//   ---  < p5
			// ^     |
			// p4    |
			//         |
			//          < p6

			constexpr auto p1 = anchor_of(1, 1);
			constexpr auto p2 = anchor_of(1, door_y);
			constexpr auto p3 = anchor_of(0, door_y);
			constexpr auto p4 = anchor_of(0, door_y + 1);
			constexpr auto p5 = anchor_of(1, door_y + 1);
			constexpr auto p6 = anchor_of(1, vertical_count - 1);

			bounding.segments.emplace_back(p1, p2);
			bounding.segments.emplace_back(p2, p3);
			bounding.segments.emplace_back(p3, p4);
			bounding.segments.emplace_back(p4, p5);
			bounding.segments.emplace_back(p5, p6);
		}
		else
		{
			// 边界
			//
			//  < p1
			// |
			// |
			// |
			// |
			// |
			// |
			// |
			//  < p2

			constexpr auto p1 = anchor_of(1, 1);
			constexpr auto p2 = anchor_of(1, vertical_count - 1);

			bounding.segments.emplace_back(p1, p2);
		}
		// 东面
		if (neighbors_value & std::to_underlying(blueprint::DirectionMask::EAST))
		{
			// 边界
			//  
			//  p1 >
			//         |  
			//         |     p3
			//         |     v
			// p2 > ---
			//          门
			// p5 > ---
			//         |    ^
			//         |    p4
			//         |
			//  p6 >

			constexpr auto p1 = anchor_of(horizontal_count - 1, 1);
			constexpr auto p2 = anchor_of(horizontal_count - 1, door_y);
			constexpr auto p3 = anchor_of(horizontal_count, door_y);
			constexpr auto p4 = anchor_of(horizontal_count, door_y + 1);
			constexpr auto p5 = anchor_of(horizontal_count - 1, door_y + 1);
			constexpr auto p6 = anchor_of(horizontal_count - 1, vertical_count - 1);

			bounding.segments.emplace_back(p1, p2);
			bounding.segments.emplace_back(p2, p3);
			bounding.segments.emplace_back(p3, p4);
			bounding.segments.emplace_back(p4, p5);
			bounding.segments.emplace_back(p5, p6);
		}
		else
		{
			// 边界
			//
			// p1 >
			//        |
			//        |
			//        |
			//        |
			//        |
			//        |
			//        |
			// p2 >

			constexpr auto p1 = anchor_of(horizontal_count - 1, 1);
			constexpr auto p2 = anchor_of(horizontal_count - 1, vertical_count - 1);

			bounding.segments.emplace_back(p1, p2);
		}

		// ===========================
		// 门
		// ===========================

		std::vector<blueprint::Door> doors{};
		// 一个邻居一个门
		doors.reserve(neighbors_count);
		{
			// 北面
			if (neighbors_value & std::to_underlying(blueprint::DirectionMask::NORTH))
			{
				doors.emplace_back(Door::standard(blueprint::Direction::NORTH)).position = center_position_of(door_x, 0);
			}
			// 南面
			if (neighbors_value & std::to_underlying(blueprint::DirectionMask::SOUTH))
			{
				doors.emplace_back(Door::standard(blueprint::Direction::SOUTH)).position = center_position_of(door_x, vertical_count - 1);
			}
			// 西面
			if (neighbors_value & std::to_underlying(blueprint::DirectionMask::WEST))
			{
				doors.emplace_back(Door::standard(blueprint::Direction::WEST)).position = center_position_of(0, door_y);
			}
			// 东面
			if (neighbors_value & std::to_underlying(blueprint::DirectionMask::EAST))
			{
				doors.emplace_back(Door::standard(blueprint::Direction::EAST)).position = center_position_of(horizontal_count - 1, door_y);
			}
		}

		// ===========================
		// 瓦片
		// ===========================

		std::vector<blueprint::Tile> tiles{};
		// 目前共有horizontal_count * vertical_count个瓦片(不考虑重叠)
		// 实际上没有那么多,尤其是我们将门单独处理后
		tiles.reserve(static_cast<std::size_t>(horizontal_count) * vertical_count);

		{
			// ============
			// 地板
			// ============
			{
				for (size_type y = 1; y < vertical_count - 1; ++y)
				{
					for (size_type x = 1; x < horizontal_count - 1; ++x)
					{
						tiles.emplace_back(Tile::floor()).position = center_position_of(x, y);
					}
				}
			}

			// ============
			// 墙壁
			// ============
			{
				// 上面/下面
				for (const size_type y: {size_type{0}, vertical_count - 1})
				{
					for (size_type x = 0; x < horizontal_count; ++x)
					{
						// 跳过中间那一格,那是门所在
						if (x == horizontal_count / 2)
						{
							if (y == 0 and (neighbors_value & std::to_underlying(blueprint::DirectionMask::NORTH)))
							{
								continue;
							}

							if (y == vertical_count - 1 and (neighbors_value & std::to_underlying(blueprint::DirectionMask::SOUTH)))
							{
								continue;
							}
						}

						tiles.emplace_back(Tile::wall()).position = center_position_of(x, y);
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
							if (x == 0 and (neighbors_value & std::to_underlying(blueprint::DirectionMask::WEST)))
							{
								continue;
							}

							if (x == horizontal_count - 1 and (neighbors_value & std::to_underlying(blueprint::DirectionMask::EAST)))
							{
								continue;
							}
						}

						tiles.emplace_back(Tile::wall()).position = center_position_of(x, y);
					}
				}
			}
		}

		// ===========================
		// 敌人生成
		// ===========================

		std::vector<blueprint::Enemy> enemies{};
		{
			enemies.reserve(3);

			enemies.emplace_back(Enemy::rat()).position = center_position_of(2, 2);
			enemies.emplace_back(Enemy::slime()).position = center_position_of(3, 3);
			enemies.emplace_back(Enemy::bat()).position = center_position_of(4, 4);
		}

		// ===========================
		// NPC生成
		// ===========================

		std::vector<blueprint::Npc> npc{};

		{
			//
		}

		// ===========================
		// 房间位置(相对) --> 世界位置(绝对)
		// ===========================

		const auto center_offset_to_world = [position](blueprint::Position& p) noexcept -> void
		{
			p.x += position.x;
			p.y += position.y;
		};

		// 边界(墙壁)
		bounding.position = position;
		// 门
		std::ranges::for_each(doors, center_offset_to_world, &blueprint::Door::position);
		// 瓦片
		std::ranges::for_each(tiles, center_offset_to_world, &blueprint::Tile::position);
		// 敌人
		std::ranges::for_each(enemies, center_offset_to_world, &blueprint::Enemy::position);
		// npc
		std::ranges::for_each(npc, center_offset_to_world, &blueprint::Npc::position);

		return
		{
				.type = type,
				.layout_position = layout_position,
				.position = position,
				.size = size,
				.bounding = std::move(bounding),
				.doors = std::move(doors),
				.tiles = std::move(tiles),
				.enemies = std::move(enemies),
				.npc = std::move(npc),
		};
	}
}
