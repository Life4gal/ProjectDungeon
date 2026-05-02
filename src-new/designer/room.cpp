// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/room.hpp>

// std::popcount
#include <bit>

#include <prometheus/platform/os.hpp>

namespace pd::designer
{
	namespace
	{
		// "door"占整个碰撞体比例
		constexpr float DoorBodyRatio = 0.6667f;
		// "sensor"占整个碰撞体比例
		constexpr float DoorSensorRatio = 0.2333f;
		// "blocker"占整个碰撞体比例
		constexpr float DoorBlockerRatio = 1 - DoorBodyRatio - DoorSensorRatio;
	}

	auto Room::standard(const size_type offset_x, const size_type offset_y, const std::underlying_type_t<RoomNeighbor> neighbors) noexcept -> blueprint::Room
	{
		const auto neighbors_count = std::popcount(neighbors);

		// ===========================================
		// FLOOR
		// ===========================================

		constexpr auto make_floor = [](const size_type tile_x, const size_type tile_y) noexcept -> blueprint::Floor
		{
			const blueprint::Transform transform
			{
					.x = static_cast<float>(tile_origin_x + tile_x * tile_width),
					.y = static_cast<float>(tile_origin_y + tile_y * tile_height),
					.scale_x = 1,
					.scale_y = 1,
					.rotation = 0,
			};
			blueprint::Sprite sprite
			{
					.texture = "./assets/tileset/floor.png",
					.x = 0,
					.y = 0,
					.width = tile_width,
					.height = tile_height,
					.origin_x = tile_origin_x,
					.origin_y = tile_origin_y,
			};

			return {.transform = transform, .sprite = std::move(sprite)};
		};

		auto floors = [] noexcept -> std::vector<blueprint::Floor>
		{
			std::vector<blueprint::Floor> fs{};
			fs.reserve(static_cast<std::size_t>(horizontal_count) * vertical_count);

			for (size_type y = 1; y < vertical_count - 1; ++y)
			{
				for (size_type x = 1; x < horizontal_count - 1; ++x)
				{
					fs.push_back(make_floor(x, y));
				}
			}

			return fs;
		}();

		// ===========================================
		// WALL
		// ===========================================

		constexpr auto make_wall = [](const size_type tile_x, const size_type tile_y) noexcept -> blueprint::Wall
		{
			const blueprint::Transform transform
			{
					.x = static_cast<float>(tile_origin_x + tile_x * tile_width),
					.y = static_cast<float>(tile_origin_y + tile_y * tile_height),
					.scale_x = 1,
					.scale_y = 1,
					.rotation = 0,
			};
			blueprint::Sprite sprite
			{
					.texture = "./assets/tileset/wall.png",
					.x = 0,
					.y = 0,
					.width = tile_width,
					.height = tile_height,
					.origin_x = tile_origin_x,
					.origin_y = tile_origin_y,
			};
			constexpr blueprint::PhysicsBody physics_body{.type = blueprint::PhysicsBodyType::STATIC, .fixed_rotation = true, .is_bullet = false};
			constexpr blueprint::PhysicsShapeBox physics_shape
			{
					.def =
					{
							.material = {.friction = 0.6f, .restitution = 0},
							.density = 0,
							.category = blueprint::PhysicsShapeType::WALL,
							.category_mask = blueprint::PhysicsShapeCollisionMask::wall,
							.is_sensor = false,
							.enable_sensor_events = false,
							.enable_contact_events = false,
					},
					.width = tile_width,
					.height = tile_height,
			};

			return {.transform = transform, .sprite = std::move(sprite), .physics_body = physics_body, .physics_shape = physics_shape};
		};

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
						if (y == 0 and (neighbors & std::to_underlying(RoomNeighbor::NORTH)))
						{
							continue;
						}

						if (y == vertical_count - 1 and (neighbors & std::to_underlying(RoomNeighbor::SOUTH)))
						{
							continue;
						}
					}

					ws.push_back(make_wall(x, y));
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
						if (x == 0 and (neighbors & std::to_underlying(RoomNeighbor::WEST)))
						{
							continue;
						}

						if (x == horizontal_count - 1 and (neighbors & std::to_underlying(RoomNeighbor::EAST)))
						{
							continue;
						}
					}

					ws.push_back(make_wall(x, y));
				}
			}

			return ws;
		}();

		// ===========================================
		// DOOR
		// ===========================================

		constexpr auto make_door = [](const size_type tile_x, const size_type tile_y, const blueprint::DoorDirection direction) noexcept -> blueprint::Door
		{
			struct desc_type
			{
				float width;
				float height;
				float offset_x;
				float offset_y;
			};

			desc_type door_desc{};
			desc_type sensor_desc{};
			desc_type blocker_desc{};

			if (direction == blueprint::DoorDirection::NORTH or direction == blueprint::DoorDirection::SOUTH)
			{
				constexpr auto half_tile_height = tile_height * 0.5f;

				door_desc.width = tile_width;
				door_desc.height = tile_height * DoorBodyRatio;
				door_desc.offset_x = 0;

				sensor_desc.width = tile_width;
				sensor_desc.height = tile_height * DoorSensorRatio;
				sensor_desc.offset_x = 0;

				blocker_desc.width = tile_width;
				blocker_desc.height = tile_height * DoorBlockerRatio;
				blocker_desc.offset_x = 0;

				if (direction == blueprint::DoorDirection::NORTH)
				{
					door_desc.offset_y = half_tile_height - door_desc.height * 0.5f;
					sensor_desc.offset_y = half_tile_height - door_desc.height - sensor_desc.height * 0.5f;
					blocker_desc.offset_y = -half_tile_height + blocker_desc.height * 0.5f;
				}
				else
				{
					door_desc.offset_y = -half_tile_height + door_desc.height * 0.5f;
					sensor_desc.offset_y = -half_tile_height + door_desc.height + sensor_desc.height * 0.5f;
					blocker_desc.offset_y = half_tile_height - blocker_desc.height * 0.5f;
				}
			}
			else if (direction == blueprint::DoorDirection::WEST or direction == blueprint::DoorDirection::EAST)
			{
				constexpr auto half_tile_width = tile_width * 0.5f;

				door_desc.width = tile_width * DoorBodyRatio;
				door_desc.height = tile_height;
				door_desc.offset_y = 0;

				sensor_desc.width = tile_width * DoorSensorRatio;
				sensor_desc.height = tile_height;
				sensor_desc.offset_y = 0;

				blocker_desc.width = tile_width * DoorBlockerRatio;
				sensor_desc.height = tile_height;
				sensor_desc.offset_y = 0;

				if (direction == blueprint::DoorDirection::WEST)
				{
					door_desc.offset_x = half_tile_width - door_desc.width * 0.5f;
					sensor_desc.offset_x = half_tile_width - door_desc.width - sensor_desc.width * 0.5f;
					blocker_desc.offset_x = -half_tile_width + blocker_desc.width * 0.5f;
				}
				else
				{
					door_desc.offset_x = -half_tile_width + door_desc.width * 0.5f;
					sensor_desc.offset_x = -half_tile_width + door_desc.width + sensor_desc.width * 0.5f;
					blocker_desc.offset_x = half_tile_width - blocker_desc.width * 0.5f;
				}
			}
			else
			{
				PROMETHEUS_PLATFORM_UNREACHABLE();
			}

			const blueprint::Transform transform
			{
					.x = static_cast<float>(tile_origin_x + tile_x * tile_width),
					.y = static_cast<float>(tile_origin_y + tile_y * tile_height),
					.scale_x = 1,
					.scale_y = 1,
					.rotation = 0,
			};
			blueprint::Sprite sprite
			{
					.texture = "./assets/tileset/door.png",
					.x = 0,
					.y = 0,
					.width = tile_width,
					.height = tile_height,
					.origin_x = tile_origin_x,
					.origin_y = tile_origin_y,
			};
			constexpr blueprint::PhysicsBody physics_body{.type = blueprint::PhysicsBodyType::STATIC, .fixed_rotation = true, .is_bullet = false};
			const blueprint::PhysicsShapeOffsetBox physics_shape_door
			{
					.def =
					{
							.material = {.friction = 0.6f, .restitution = 0},
							.density = 0,
							.category = blueprint::PhysicsShapeType::DOOR,
							.category_mask = blueprint::PhysicsShapeCollisionMask::door_close,
							.is_sensor = false,
							.enable_sensor_events = false,
							.enable_contact_events = true,
					},
					.width = door_desc.width,
					.height = door_desc.height,
					.offset_x = door_desc.offset_x,
					.offset_y = door_desc.offset_y,
			};
			const blueprint::PhysicsShapeOffsetBox physics_shape_sensor
			{
					.def =
					{
							.material = {.friction = 0, .restitution = 0},
							.density = 0,
							.category = blueprint::PhysicsShapeType::DOOR,
							.category_mask = blueprint::PhysicsShapeCollisionMask::door_sensor,
							.is_sensor = true,
							.enable_sensor_events = true,
							.enable_contact_events = false,
					},
					.width = sensor_desc.width,
					.height = sensor_desc.height,
					.offset_x = sensor_desc.offset_x,
					.offset_y = sensor_desc.offset_y,
			};
			const blueprint::PhysicsShapeOffsetBox physics_shape_blocker
			{
					.def =
					{
							.material = {.friction = 0, .restitution = 0},
							.density = 0,
							.category = blueprint::PhysicsShapeType::DOOR,
							.category_mask = blueprint::PhysicsShapeCollisionMask::door_blocker,
							.is_sensor = false,
							.enable_sensor_events = false,
							.enable_contact_events = false,
					},
					.width = blocker_desc.width,
					.height = blocker_desc.height,
					.offset_x = blocker_desc.offset_x,
					.offset_y = blocker_desc.offset_y,
			};

			return {
					.transform = transform,
					.sprite = std::move(sprite),
					.physics_body = physics_body,
					.physics_shape_door = physics_shape_door,
					.physics_shape_sensor = physics_shape_sensor,
					.physics_shape_blocker = physics_shape_blocker,
					.direction = direction
			};
		};

		auto doors = [&] noexcept -> std::vector<blueprint::Door>
		{
			std::vector<blueprint::Door> ds;
			ds.reserve(neighbors_count);

			if (neighbors & std::to_underlying(RoomNeighbor::NORTH))
			{
				ds.push_back(make_door(horizontal_count / 2, 0, blueprint::DoorDirection::NORTH));
			}
			if (neighbors & std::to_underlying(RoomNeighbor::SOUTH))
			{
				ds.push_back(make_door(horizontal_count / 2, vertical_count - 1, blueprint::DoorDirection::SOUTH));
			}
			if (neighbors & std::to_underlying(RoomNeighbor::WEST))
			{
				ds.push_back(make_door(0, vertical_count / 2, blueprint::DoorDirection::WEST));
			}
			if (neighbors & std::to_underlying(RoomNeighbor::EAST))
			{
				ds.push_back(make_door(horizontal_count - 1, vertical_count / 2, blueprint::DoorDirection::EAST));
			}

			return ds;
		}();

		// ===========================================
		// ROOM
		// ===========================================

		return {.offset_x = static_cast<float>(offset_x * width), .offset_y = static_cast<float>(offset_y * height), .floors = std::move(floors), .walls = std::move(walls), .doors = std::move(doors)};
	}
}
