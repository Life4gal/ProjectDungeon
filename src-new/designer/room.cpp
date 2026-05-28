// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/room.hpp>

#include <algorithm>
// std::popcount
#include <bit>

#include <designer/enemy.hpp>

#include <prometheus/platform/os.hpp>

namespace pd::designer
{
	namespace
	{
		// 感应区域占比
		constexpr auto SensorAreaRatio = 0.2f;
		// 感应区域大小
		constexpr auto SensorAreaWidth = Room::tile_width * SensorAreaRatio;
		constexpr auto SensorAreaHeight = Room::tile_height * SensorAreaRatio;

		// // 门区域占比
		// constexpr auto DoorAreaRatio = 1.0f - SensorAreaRatio;
		// // 门区域大小
		// constexpr auto DoorAreaWidth = Room::tile_width * DoorAreaRatio;
		// constexpr auto DoorAreaHeight = Room::tile_height * DoorAreaRatio;
	}

	auto Room::standard(const size_type offset_x, const size_type offset_y, const blueprint::DirectionMask neighbors) noexcept -> blueprint::Room
	{
		// ===========================
		// 类型 + 邻居
		// ===========================

		constexpr auto type = blueprint::RoomType::STANDARD;

		const auto neighbors_value = std::to_underlying(neighbors);
		const auto neighbors_count = std::popcount(neighbors_value);

		// ===========================
		// 位置 + 大小
		// ===========================

		const auto room_x = static_cast<float>(offset_x * width);
		const auto room_y = static_cast<float>(offset_y * height);

		const blueprint::LayoutPosition layout_position{.x = offset_x, .y = offset_y};
		const blueprint::Position position{.x = room_x, .y = room_y};
		constexpr blueprint::Size size{.width = width, .height = height};

		// ===========================
		// 门 + 房间边界
		// ===========================

		std::array<blueprint::Door, 4> doors{};
		blueprint::Bounding bounding{};

		bounding.position = position;
		// 边界没有邻居 --> 一条线段
		// 边界有邻居 --> 四条线段
		bounding.segments.reserve(4 + 3 * neighbors_count);
		{
			using blueprint::Position;

			const blueprint::DynamicSprite sprite
			{
					.frames =
					{
							// 第一帧
							{.texture = "./assets/tileset/door.png", .position = {.x = 0, .y = 0}},
							// 第二帧
							{.texture = "./assets/tileset/door.png", .position = {.x = 64, .y = 0}},
							// 第三帧
							{.texture = "./assets/tileset/door.png", .position = {.x = 128, .y = 0}},
							// 第四帧
							{.texture = "./assets/tileset/door.png", .position = {.x = 192, .y = 0}},
					},
					.size = {.width = 64, .height = 64},
					.origin = {.x = 32, .y = 32},
					.duration_ms = 100,
					.looping = false,
					.reversed = false,
					.pause = true,
			};

			if (neighbors_value & std::to_underlying(blueprint::DirectionMask::NORTH))
			{
				constexpr auto x = (horizontal_count / 2) * tile_width;

				auto& door = doors[std::to_underlying(blueprint::Direction::NORTH)];

				// 门位置
				door.position.x = static_cast<float>(x) + sprite.origin.x;
				door.position.y = 0 + sprite.origin.y;

				// 门感应区位置
				door.sensor_position.x = 0;
				door.sensor_position.y = -sprite.origin.y + SensorAreaHeight / 2;

				// 门大小
				door.size.width = tile_width;
				door.size.height = tile_height;

				// 门感应区大小
				door.sensor_size.width = tile_width;
				door.sensor_size.height = SensorAreaHeight;

				// 门渲染
				door.sprite = sprite;

				// 边界
				//
				//         p3 >    < p4
				//                |门|
				//      -------     -------
				//     ^       ^   ^       ^
				//     p1     p2  p5     p6

				constexpr Position p1{.x = tile_width, .y = tile_height};
				constexpr Position p2{.x = static_cast<float>(x), .y = tile_height};
				constexpr Position p3{.x = static_cast<float>(x), .y = 0};
				constexpr Position p4{.x = static_cast<float>(x) + tile_width, .y = 0};
				constexpr Position p5{.x = static_cast<float>(x) + tile_width, .y = tile_height};
				constexpr Position p6{.x = width - tile_width, .y = tile_height};

				bounding.segments.emplace_back(p1, p2);
				bounding.segments.emplace_back(p2, p3);
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

				constexpr Position p1{.x = tile_width, .y = tile_height};
				constexpr Position p2{.x = width - tile_width, .y = height - tile_height};

				bounding.segments.emplace_back(p1, p2);
			}

			if (neighbors_value & std::to_underlying(blueprint::DirectionMask::SOUTH))
			{
				constexpr auto x = (horizontal_count / 2) * tile_width;

				auto& door = doors[std::to_underlying(blueprint::Direction::SOUTH)];

				// 门位置
				door.position.x = static_cast<float>(x) + sprite.origin.x;
				door.position.y = height - tile_height + sprite.origin.y;

				// 门感应区位置
				door.sensor_position.x = 0;
				door.sensor_position.y = sprite.origin.y - SensorAreaHeight / 2;

				// 门大小
				door.size.width = tile_width;
				door.size.height = tile_height;

				// 门感应区大小
				door.sensor_size.width = tile_width;
				door.sensor_size.height = SensorAreaHeight;

				// 门渲染
				door.sprite = sprite;

				// 边界
				//
				//      p1    p2  p5     p6
				//      v       v    v       v
				//      -------     -------
				//                |门|
				//        p3 >     < p4

				constexpr Position p1{.x = tile_width, .y = height - tile_height};
				constexpr Position p2{.x = static_cast<float>(x), .y = height - tile_height};
				constexpr Position p3{.x = static_cast<float>(x), .y = height};
				constexpr Position p4{.x = static_cast<float>(x) + tile_width, .y = height};
				constexpr Position p5{.x = static_cast<float>(x) + tile_width, .y = height - tile_height};
				constexpr Position p6{.x = width - tile_width, .y = height - tile_height};

				bounding.segments.emplace_back(p1, p2);
				bounding.segments.emplace_back(p2, p3);
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

				constexpr Position p1{.x = tile_width, .y = height - tile_height};
				constexpr Position p2{.x = width - tile_width, .y = height - tile_height};

				bounding.segments.emplace_back(p1, p2);
			}

			if (neighbors_value & std::to_underlying(blueprint::DirectionMask::WEST))
			{
				constexpr auto y = (vertical_count / 2) * tile_height;

				auto& door = doors[std::to_underlying(blueprint::Direction::WEST)];

				// 门位置
				door.position.x = 0 + sprite.origin.x;
				door.position.y = static_cast<float>(y) + sprite.origin.y;

				// 门感应区位置
				door.sensor_position.x = -sprite.origin.x + SensorAreaWidth / 2;
				door.sensor_position.y = 0;

				// 门大小
				door.size.width = tile_width;
				door.size.height = tile_height;

				// 门感应区大小
				door.sensor_size.width = SensorAreaWidth;
				door.sensor_size.height = tile_height;

				// 门渲染
				door.sprite = sprite;

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

				constexpr Position p1{.x = tile_width, .y = tile_height};
				constexpr Position p2{.x = tile_width, .y = static_cast<float>(y)};
				constexpr Position p3{.x = 0, .y = static_cast<float>(y)};
				constexpr Position p4{.x = 0, .y = static_cast<float>(y) + tile_height};
				constexpr Position p5{.x = tile_width, .y = static_cast<float>(y) + tile_height};
				constexpr Position p6{.x = tile_width, .y = height - tile_height};

				bounding.segments.emplace_back(p1, p2);
				bounding.segments.emplace_back(p2, p3);
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

				constexpr Position p1{.x = tile_width, .y = tile_height};
				constexpr Position p2{.x = tile_width, .y = height - tile_height};

				bounding.segments.emplace_back(p1, p2);
			}

			if (neighbors_value & std::to_underlying(blueprint::DirectionMask::EAST))
			{
				constexpr auto y = (vertical_count / 2) * tile_height;

				auto& door = doors[std::to_underlying(blueprint::Direction::EAST)];

				// 门位置
				door.position.x = width - tile_width + sprite.origin.x;
				door.position.y = static_cast<float>(y) + sprite.origin.y;

				// 门感应区位置
				door.sensor_position.x = sprite.origin.x - SensorAreaWidth / 2;
				door.sensor_position.y = 0;

				// 门大小
				door.size.width = tile_width;
				door.size.height = tile_height;

				// 门感应区大小
				door.sensor_size.width = SensorAreaWidth;
				door.sensor_size.height = tile_height;

				// 门渲染
				door.sprite = sprite;

				// 边界
				//  
				//   p1 >
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

				constexpr Position p1{.x = width - tile_width, .y = tile_height};
				constexpr Position p2{.x = width - tile_width, .y = static_cast<float>(y)};
				constexpr Position p3{.x = width, .y = static_cast<float>(y)};
				constexpr Position p4{.x = width, .y = static_cast<float>(y) + tile_height};
				constexpr Position p5{.x = width - tile_width, .y = static_cast<float>(y) + tile_height};
				constexpr Position p6{.x = width - tile_width, .y = height - tile_height};

				bounding.segments.emplace_back(p1, p2);
				bounding.segments.emplace_back(p2, p3);
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

				constexpr Position p1{.x = width - tile_width, .y = tile_height};
				constexpr Position p2{.x = width - tile_width, .y = height - tile_height};

				bounding.segments.emplace_back(p1, p2);
			}
		}

		// ===========================
		// 房间内所有瓦片
		// ===========================

		std::vector<blueprint::Tile> tiles{};

		{
			// 目前共有horizontal_count * vertical_count个瓦片(不考虑重叠)
			// 实际上没有那么多,尤其是我们将门单独处理后
			constexpr auto total_tiles = horizontal_count * vertical_count;
			tiles.reserve(total_tiles);

			// ============
			// 地板
			// ============
			{
				blueprint::Tile tile
				{
						.position = {.x = 0, .y = 0},
						.sprite =
						{
								.sprite = blueprint::StaticSprite
								{
										.texture = "./assets/tileset/floor.png",
										.position = {.x = 0, .y = 0},
										.size = {.width = tile_width, .height = tile_height},
										.origin = {.x = tile_origin_x, .y = tile_origin_y},
								},
								.render_layer = blueprint::RenderLayer::FLOOR,
						},
						.collision = std::nullopt,
				};

				for (size_type y = 1; y < vertical_count - 1; ++y)
				{
					for (size_type x = 1; x < horizontal_count - 1; ++x)
					{
						tile.position.x = static_cast<float>(tile_origin_x + x * tile_width);
						tile.position.y = static_cast<float>(tile_origin_y + y * tile_height);

						tiles.push_back(tile);
					}
				}
			}

			// ============
			// 墙壁
			// ============
			{
				blueprint::Tile tile
				{
						.position = {.x = 0, .y = 0},
						.sprite =
						{
								.sprite = blueprint::StaticSprite
								{
										.texture = "./assets/tileset/wall.png",
										.position = {.x = 0, .y = 0},
										.size = {.width = tile_width, .height = tile_height},
										.origin = {.x = tile_origin_x, .y = tile_origin_y},
								},
								.render_layer = blueprint::RenderLayer::FLOOR,
						},
						.collision =
						// 墙壁的阻挡作用已被房间边界取代
						// 除非是位于房间内的墙壁?
						std::nullopt,
						// blueprint::Collision
						// {
						// 		.def =
						// 		{
						// 				.type = blueprint::CollisionBodyType::STATIC,
						// 				.fixed_rotation = true,
						// 				.is_bullet = false,
						// 		},
						// 		.shapes =
						// 		{
						// 				// 矩形碰撞体
						// 				{
						// 						.def =
						// 						{
						// 								.material = {.friction = 0.6f, .restitution = 0},
						// 								.density = 0,
						// 								.category = blueprint::CollisionCategory::WALL,
						// 								.mask = blueprint::CollisionMask::WALL,
						// 								.is_sensor = false,
						// 								.enable_sensor_events = false,
						// 								.enable_contact_events = false,
						// 						},
						// 						.shape =
						// 						blueprint::CollisionShape::box
						// 						{
						// 								.size = {.width = tile_width, .height = tile_height},
						// 						},
						// 				},
						// 				//
						// 		},
						// },
				};

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

						tile.position.x = static_cast<float>(tile_origin_x + x * tile_width);
						tile.position.y = static_cast<float>(tile_origin_y + y * tile_height);

						tiles.push_back(tile);
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

						tile.position.x = static_cast<float>(tile_origin_x + x * tile_width);
						tile.position.y = static_cast<float>(tile_origin_y + y * tile_height);

						tiles.push_back(tile);
					}
				}
			}
		}

		// ===========================
		// 房间内所有敌人
		// ===========================

		std::vector<blueprint::Enemy> enemies{};

		{
			// enemies.reserve(3);
			//
			// enemies.push_back(Enemy::rat(2, 2));
			// enemies.push_back(Enemy::slime(3, 3));
			// enemies.push_back(Enemy::bat(4, 4));

			// TODO: 测试用
			auto rat = Enemy::rat(2, 2);
			rat.ai.move_behavior.behavior = blueprint::MoveBehavior::stationary{};
			enemies.push_back(std::move(rat));
		}

		// ===========================
		// 房间内所有NPC
		// ===========================

		std::vector<blueprint::Npc> npc{};

		{
			//
		}

		// ===========================
		// 房间位置(相对) --> 世界位置(绝对)
		// ===========================

		const auto process_offset = [room_x, room_y](blueprint::Position& p) noexcept -> void
		{
			p.x += room_x;
			p.y += room_y;
		};

		// 门感应区
		std::ranges::for_each(doors, process_offset, &blueprint::Door::position);
		// 瓦片
		std::ranges::for_each(tiles, process_offset, &blueprint::Tile::position);
		// 敌人
		std::ranges::for_each(enemies, process_offset, &blueprint::Enemy::position);
		// npc
		std::ranges::for_each(npc, process_offset, &blueprint::Npc::position);

		return
		{
				.type = type,
				.neighbors = neighbors,
				.layout_position = layout_position,
				.position = position,
				.size = size,
				.doors = std::move(doors),
				.bounding = std::move(bounding),
				.tiles = std::move(tiles),
				.enemies = std::move(enemies),
				.npc = std::move(npc),
		};
	}
}
