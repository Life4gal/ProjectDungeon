// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/player.hpp>

namespace pd::designer
{
	auto Player::test_character() noexcept -> blueprint::Player
	{
		constexpr blueprint::Transform transform
		{
				.x = 0,
				.y = 0,
				.scale_x = 1,
				.scale_y = 1,
				.rotation = 0,
		};
		blueprint::SpriteAnimation animation
		{
				.frames =
				{
						// 第一帧
						{.texture = "./assets/tileset/player.png", .x = 0, .y = 0, .width = 64, .height = 64, .origin_x = 32, .origin_y = 32},
						// 第二帧
						{.texture = "./assets/tileset/player.png", .x = 64, .y = 0, .width = 64, .height = 64, .origin_x = 32, .origin_y = 32},
						// 第三帧
						{.texture = "./assets/tileset/player.png", .x = 128, .y = 0, .width = 64, .height = 64, .origin_x = 32, .origin_y = 32},
						// 第四帧
						{.texture = "./assets/tileset/player.png", .x = 192, .y = 0, .width = 64, .height = 64, .origin_x = 32, .origin_y = 32},
				},
				.durations_ms = {250, 250, 250, 250},
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

		return {.transform = transform, .animation = std::move(animation), .physics_body = physics_body, .physics_shape = physics_shape, .actor = actor};
	}
}
