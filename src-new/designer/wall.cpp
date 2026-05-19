// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/wall.hpp>

#include <designer/room.hpp>

namespace pd::designer
{
	namespace
	{
		constexpr blueprint::BodyDesc BodyDesc
		{
				.type = blueprint::BodyType::STATIC,
				.fixed_rotation = true,
				.is_bullet = false,
		};
		constexpr blueprint::ShapeDesc ShapeDesc
		{
				.material = {.friction = 0.6f, .restitution = 0},
				.density = 0,
				.category = blueprint::ShapeType::WALL,
				.category_mask = blueprint::CollisionMask::wall,
				.is_sensor = false,
				.enable_sensor_events = false,
				.enable_contact_events = false,
		};
		constexpr blueprint::ShapeCategory::Box Shape
		{
				.size = {.width = Room::tile_width, .height = Room::tile_height},
		};
	}

	auto Wall::standard(const size_type tile_x, const size_type tile_y) noexcept -> blueprint::Wall
	{
		blueprint::Sprite sprite
		{
				.texture = "./assets/tileset/wall.png",
				.position = {.x = 0, .y = 0},
				.size = {.width = Room::tile_width, .height = Room::tile_height},
				.origin = {.x = Room::tile_origin_x, .y = Room::tile_origin_y},
		};
		const blueprint::Position position
		{
				.x = static_cast<float>(Room::tile_origin_x + tile_x * Room::tile_width),
				.y = static_cast<float>(Room::tile_origin_y + tile_y * Room::tile_height),
		};

		return {.sprite = std::move(sprite), .position = position, .body_desc = BodyDesc, .shape_desc = ShapeDesc, .shape = Shape};
	}
}
