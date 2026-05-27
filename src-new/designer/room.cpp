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
		// 门感应区 + 房间边界
		// ===========================

		std::array<blueprint::Room::DoorSensor, 4> door_sensors{};
		std::vector<blueprint::Room::Bounding> bounding{};

		{
			using blueprint::Position;

			// 边界没有邻居 --> 一条线段
			// 边界有邻居 --> 四条线段
			bounding.reserve(4 + 3 * neighbors_count);

			if (neighbors_value & std::to_underlying(blueprint::DirectionMask::NORTH))
			{
				constexpr auto x = (horizontal_count / 2) * tile_width;

				// 感应区
				auto& sensor = door_sensors[std::to_underlying(blueprint::Direction::NORTH)];
				sensor =
				{
						.shape = {.size = {.width = tile_width, .height = SensorAreaHeight}},
						.position = {.x = static_cast<float>(x), .y = 0},
				};

				// 边界
				constexpr Position p1{.x = tile_width, .y = tile_height};
				const Position p2{.x = sensor.position.x, .y = tile_height};
				const Position p3{.x = sensor.position.x, .y = 0};

				bounding.push_back({p1, p2});
				bounding.push_back({p2, p3});

				const Position p4{.x = sensor.position.x + sensor.shape.size.width, .y = 0};
				const Position p5{.x = sensor.position.x + sensor.shape.size.width, .y = tile_height};
				constexpr Position p6{.x = width - tile_width, .y = tile_height};

				bounding.push_back({p4, p5});
				bounding.push_back({p5, p6});
			}
			else
			{
				// 边界
				constexpr Position p1{.x = tile_width, .y = tile_height};
				constexpr Position p2{.x = width - tile_width, .y = height - tile_height};

				bounding.push_back({p1, p2});
			}

			if (neighbors_value & std::to_underlying(blueprint::DirectionMask::SOUTH))
			{
				constexpr auto x = (horizontal_count / 2) * tile_width;

				// 感应区
				auto& sensor = door_sensors[std::to_underlying(blueprint::Direction::SOUTH)];
				sensor =
				{
						.shape = {.size = {.width = tile_width, .height = SensorAreaHeight}},
						.position = {.x = static_cast<float>(x), .y = height - SensorAreaHeight},
				};

				// 边界
				constexpr Position p1{.x = tile_width, .y = height - tile_height};
				const Position p2{.x = sensor.position.x, .y = height - tile_height};
				const Position p3{.x = sensor.position.x, .y = height};

				bounding.push_back({p1, p2});
				bounding.push_back({p2, p3});

				const Position p4{.x = sensor.position.x + sensor.shape.size.width, .y = height};
				const Position p5{.x = sensor.position.x + sensor.shape.size.width, .y = height - tile_height};
				constexpr Position p6{.x = width - tile_width, .y = height - tile_height};

				bounding.push_back({p4, p5});
				bounding.push_back({p5, p6});
			}
			else
			{
				// 边界
				constexpr Position p1{.x = tile_width, .y = height - tile_height};
				constexpr Position p2{.x = width - tile_width, .y = height - tile_height};

				bounding.push_back({p1, p2});
			}

			if (neighbors_value & std::to_underlying(blueprint::DirectionMask::WEST))
			{
				constexpr auto y = (vertical_count / 2) * tile_height;

				// 感应区
				auto& sensor = door_sensors[std::to_underlying(blueprint::Direction::WEST)];
				sensor =
				{
						.shape = {.size = {.width = SensorAreaWidth, .height = tile_height}},
						.position = {.x = 0, .y = static_cast<float>(y)},
				};

				// 边界
				constexpr Position p1{.x = tile_width, .y = tile_height};
				const Position p2{.x = tile_width, .y = sensor.position.y};
				const Position p3{.x = 0, .y = sensor.position.y};

				bounding.push_back({p1, p2});
				bounding.push_back({p2, p3});

				const Position p4{.x = 0, .y = sensor.position.y + sensor.shape.size.height};
				const Position p5{.x = tile_width, .y = sensor.position.y + sensor.shape.size.height};
				constexpr Position p6{.x = tile_width, .y = height - tile_height};

				bounding.push_back({p4, p5});
				bounding.push_back({p5, p6});
			}
			else
			{
				// 边界
				constexpr Position p1{.x = tile_width, .y = tile_height};
				constexpr Position p2{.x = tile_width, .y = height - tile_height};

				bounding.push_back({p1, p2});
			}

			if (neighbors_value & std::to_underlying(blueprint::DirectionMask::EAST))
			{
				constexpr auto x = width - SensorAreaWidth;
				constexpr auto y = (vertical_count / 2) * tile_height;

				// 感应区
				auto& sensor = door_sensors[std::to_underlying(blueprint::Direction::EAST)];
				sensor =
				{
						.shape = {.size = {.width = SensorAreaWidth, .height = tile_height}},
						.position = {.x = x, .y = static_cast<float>(y)},
				};

				// 边界
				constexpr Position p1{.x = width - tile_width, .y = tile_height};
				const Position p2{.x = width - tile_width, .y = sensor.position.y};
				const Position p3{.x = width, .y = sensor.position.y};

				bounding.push_back({p1, p2});
				bounding.push_back({p2, p3});

				const Position p4{.x = width, .y = sensor.position.y + sensor.shape.size.height};
				const Position p5{.x = width - tile_width, .y = sensor.position.y + sensor.shape.size.height};
				constexpr Position p6{.x = width - tile_width, .y = height - tile_height};

				bounding.push_back({p4, p5});
				bounding.push_back({p5, p6});
			}
			else
			{
				// 边界
				constexpr Position p1{.x = width - tile_width, .y = tile_height};
				constexpr Position p2{.x = width - tile_width, .y = height - tile_height};

				bounding.push_back({p1, p2});
			}
		}

		// ===========================
		// 房间内所有瓦片
		// ===========================

		std::vector<blueprint::Tile> tiles{};

		{
			// 目前共有horizontal_count * vertical_count个瓦片(不考虑重叠)
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
								blueprint::StaticSprite
								{
										.texture = "./assets/tileset/floor.png",
										.position = {.x = 0, .y = 0},
										.size = {.width = tile_width, .height = tile_height},
										.origin = {.x = tile_origin_x, .y = tile_origin_y},
								},
								blueprint::RenderLayer::FLOOR,
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
						blueprint::Collision
						{
								.def =
								{
										.type = blueprint::CollisionBodyType::STATIC,
										.fixed_rotation = true,
										.is_bullet = false,
								},
								.shapes =
								{
										// 矩形碰撞体
										{
												.def =
												{
														.material = {.friction = 0.6f, .restitution = 0},
														.density = 0,
														.category = blueprint::CollisionCategory::WALL,
														.mask = blueprint::CollisionMask::WALL,
														.is_sensor = false,
														.enable_sensor_events = false,
														.enable_contact_events = false,
												},
												.shape =
												blueprint::CollisionShape::box
												{
														.size = {.width = tile_width, .height = tile_height},
												},
										},
										//
								},
						},
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

			// ============
			// 门
			// ============
			{
				blueprint::Tile tile
				{
						.position = {.x = 0, .y = 0},
						.sprite =
						{
								// TODO: 理论上门应该是动态精灵(只不过起始为暂停状态)
								.sprite = blueprint::StaticSprite
								{
										.texture = "./assets/tileset/door.png",
										.position = {.x = 0, .y = 0},
										.size = {.width = tile_width, .height = tile_height},
										.origin = {.x = tile_origin_x, .y = tile_origin_y},
								},
								.render_layer = blueprint::RenderLayer::DOOR,
						},
						.collision =
						blueprint::Collision
						{
								.def =
								{
										.type = blueprint::CollisionBodyType::STATIC,
										.fixed_rotation = false,
										.is_bullet = false,
								},
								.shapes = {
										// 矩形碰撞体
										{
												.def =
												{
														.material = {.friction = 0.6f, .restitution = 0},
														.density = 0,
														.category = blueprint::CollisionCategory::DOOR,
														.mask = blueprint::CollisionMask::DOOR,
														.is_sensor = false,
														.enable_sensor_events = false,
														// 允许接触事件
														.enable_contact_events = true,
												},
												// 大小懒得算,直接和感应区重叠也无所谓 :)
												.shape =
												blueprint::CollisionShape::box
												{
														.size = {.width = tile_width, .height = tile_height},
												},
										},
										//
								},
						},
				};

				if (neighbors_value & std::to_underlying(blueprint::DirectionMask::NORTH))
				{
					constexpr auto x = tile_origin_x + (horizontal_count / 2) * tile_width;
					constexpr auto y = tile_origin_y + 0 * tile_height;

					tile.position = {.x = static_cast<float>(x), .y = static_cast<float>(y)};

					tiles.push_back(tile);
				}

				if (neighbors_value & std::to_underlying(blueprint::DirectionMask::SOUTH))
				{
					constexpr auto x = tile_origin_x + (horizontal_count / 2) * tile_width;
					constexpr auto y = tile_origin_y + (vertical_count - 1) * tile_height;

					tile.position = {.x = static_cast<float>(x), .y = static_cast<float>(y)};

					tiles.push_back(tile);
				}

				if (neighbors_value & std::to_underlying(blueprint::DirectionMask::WEST))
				{
					constexpr auto x = tile_origin_x + 0 * tile_width;
					constexpr auto y = tile_origin_y + (vertical_count / 2) * tile_height;

					tile.position = {.x = static_cast<float>(x), .y = static_cast<float>(y)};

					tiles.push_back(tile);
				}

				if (neighbors_value & std::to_underlying(blueprint::DirectionMask::EAST))
				{
					constexpr auto x = tile_origin_x + (horizontal_count - 1) * tile_width;
					constexpr auto y = tile_origin_y + (vertical_count / 2) * tile_height;

					tile.position = {.x = static_cast<float>(x), .y = static_cast<float>(y)};

					tiles.push_back(tile);
				}
			}
		}

		// ===========================
		// 房间内所有敌人
		// ===========================

		std::vector<blueprint::Enemy> enemies{};

		{
			enemies.reserve(3);

			enemies.push_back(Enemy::rat(2, 2));
			enemies.push_back(Enemy::slime(3, 3));
			enemies.push_back(Enemy::bat(4, 4));
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
		std::ranges::for_each(door_sensors, process_offset, &blueprint::Room::DoorSensor::position);
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
				.door_sensors = door_sensors,
				.bounding = std::move(bounding),
				.tiles = std::move(tiles),
				.enemies = std::move(enemies),
				.npc = std::move(npc),
		};
	}
}
