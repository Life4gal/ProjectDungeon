// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/wall.hpp>

#include <designer/room.hpp>

namespace pd::designer
{
	auto Wall::standard(const size_type tile_x, const size_type tile_y) noexcept -> blueprint::Wall
	{
		const blueprint::Transform transform
		{
				.x = static_cast<float>(Room::tile_origin_x + tile_x * Room::tile_width),
				.y = static_cast<float>(Room::tile_origin_y + tile_y * Room::tile_height),
				.scale_x = 1,
				.scale_y = 1,
				.rotation = 0,
		};
		blueprint::Sprite sprite
		{
				.texture = "./assets/tileset/wall.png",
				.x = 0,
				.y = 0,
				.width = Room::tile_width,
				.height = Room::tile_height,
				.origin_x = Room::tile_origin_x,
				.origin_y = Room::tile_origin_y,
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
				.width = Room::tile_width,
				.height = Room::tile_height,
		};

		return {.transform = transform, .sprite = std::move(sprite), .physics_body = physics_body, .physics_shape = physics_shape};
	}
}
