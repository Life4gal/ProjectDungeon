// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/room.hpp>

namespace pd::designer
{
	auto Room::standard(const size_type offset_x, const size_type offset_y) noexcept -> blueprint::Room
	{
		constexpr auto make_floor = [](const size_type tile_x, const size_type tile_y) noexcept -> blueprint::Floor
		{
			return
			{
					// clang-format off
					.transform = {.x = static_cast<float>(tile_origin_x + tile_x * tile_width), .y = static_cast<float>(tile_origin_y + tile_y * tile_height), .scale_x = 1, .scale_y = 1, .rotation = 0},
					// clang-format on
					.sprite = {.texture = "./assets/tileset/floor.png", .x = 0, .y = 0, .width = tile_width, .height = tile_height, .origin_x = tile_origin_x, .origin_y = tile_origin_y},
			};
		};
		constexpr auto make_floors = [] noexcept -> std::vector<blueprint::Floor>
		{
			std::vector<blueprint::Floor> floors{};
			floors.reserve(static_cast<std::size_t>(horizontal_count) * vertical_count);

			for (size_type y = 1; y < vertical_count - 1; ++y)
			{
				for (size_type x = 1; x < horizontal_count - 1; ++x)
				{
					floors.push_back(make_floor(x, y));
				}
			}

			return floors;
		};

		constexpr auto make_wall = [](const size_type tile_x, const size_type tile_y) noexcept -> blueprint::Wall
		{
			return
			{
					// clang-format off
					.transform = {.x = static_cast<float>(tile_origin_x + tile_x * tile_width), .y = static_cast<float>(tile_origin_y + tile_y * tile_height), .scale_x = 1, .scale_y = 1, .rotation = 0},
					// clang-format on
					.sprite = {.texture = "./assets/tileset/wall.png", .x = 0, .y = 0, .width = tile_width, .height = tile_height, .origin_x = tile_origin_x, .origin_y = tile_origin_y},
					.physics_body = {.type = blueprint::PhysicsBodyType::STATIC, .is_bullet = false},
					// clang-format off
					.physics_shape = {.def = {.material = {.friction = 0, .restitution = 0}, .is_sensor = false, .enable_sensor_events = false, .enable_contact_events = false}, .width = tile_width, .height =tile_height},
					// clang-format on
			};
		};
		constexpr auto make_walls = [] noexcept -> std::vector<blueprint::Wall>
		{
			std::vector<blueprint::Wall> walls{};
			walls.reserve(horizontal_count * 2 + (vertical_count - 2) * 2);

			// 上面/下面
			for (const size_type y: {size_type{0}, vertical_count - 1})
			{
				for (size_type x = 0; x < horizontal_count; ++x)
				{
					walls.push_back(make_wall(x, y));
				}
			}
			// 左面/右面
			for (const size_type x: {size_type{0}, horizontal_count - 1})
			{
				for (size_type y = 1; y < vertical_count - 1; ++y)
				{
					walls.push_back(make_wall(x, y));
				}
			}

			return walls;
		};

		return {.offset_x = static_cast<float>(offset_x * width), .offset_y = static_cast<float>(offset_y * height), .floors = make_floors(), .walls = make_walls(),};
	}
}
