// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/player.hpp>

#include <designer/room.hpp>

namespace pd::designer
{
	auto Player::test_character() noexcept -> blueprint::Player
	{
		constexpr blueprint::Position position
		{
				.x = static_cast<float>(Room::tile_origin_x + 10 * Room::tile_width),
				.y = static_cast<float>(Room::tile_origin_y + 5 * Room::tile_height),
		};
		blueprint::SpriteAnimation animation
		{
				.frames =
				{
						// 第一帧
						{.texture = "./assets/tileset/player.png", .position = {.x = 0, .y = 0}},
						// 第二帧
						{.texture = "./assets/tileset/player.png", .position = {.x = 64, .y = 0}},
						// 第三帧
						{.texture = "./assets/tileset/player.png", .position = {.x = 128, .y = 0}},
						// 第四帧
						{.texture = "./assets/tileset/player.png", .position = {.x = 192, .y = 0}},
				},
				.size = {.width = 64, .height = 64},
				.origin = {.x = 32, .y = 32},
				.scale = {.x = 1, .y = 1},
				.duration_ms = 250,
				.looping = true,
				.reversed = false,
		};
		constexpr blueprint::PhysicsBody physics_body{.type = blueprint::PhysicsBodyType::DYNAMIC, .fixed_rotation = true, .is_bullet = false};
		constexpr blueprint::PhysicsShapeCircle physics_shape
		{
				.def =
				{
						.material = {.friction = 0.3f, .restitution = 0},
						.density = 1,
						.category = blueprint::PhysicsShapeType::PLAYER,
						.category_mask = blueprint::PhysicsShapeCollisionMask::player,
						.is_sensor = false,
						.enable_sensor_events = true,
						.enable_contact_events = true,
				},
				.radius = 32,
		};
		constexpr blueprint::Actor actor{.health = 50, .mana = 20, .speed = 120};

		return {.position = position, .animation = std::move(animation), .actor = actor, .physics_body = physics_body, .physics_shape = physics_shape};
	}
}
