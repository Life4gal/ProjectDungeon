// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/player.hpp>

namespace pd::designer
{
	auto Player::test_character() noexcept -> blueprint::Player
	{
		return
		{
				.transform = {.x = 0, .y = 0, .scale_x = 1, .scale_y = 1, .rotation = 0},
				.animation =
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
				},
				.physics_body = {.type = blueprint::PhysicsBodyType::DYNAMIC, .is_bullet = false},
				// clang-format off
				.physics_shape = {.def = {.material = {.friction = 0.5f, .restitution = 0.1f}, .is_sensor = false, .enable_sensor_events = false, .enable_contact_events = true}, .radius = 32},
				// clang-format on
		};
	}
}
