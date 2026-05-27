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
		blueprint::DynamicSprite sprite
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
				.duration_ms = 250,
				.looping = true,
				.reversed = false,
		};
		blueprint::Collision collision
		{
				.def =
				{
						.type = blueprint::CollisionBodyType::DYNAMIC,
						.fixed_rotation = true,
						.is_bullet = false,
				},
				.shapes =
				{
						// 圆形碰撞体
						{
								.def =
								{
										.material = {.friction = 0.3f, .restitution = 0},
										.density = 1,
										.category = blueprint::CollisionCategory::PLAYER,
										.mask = blueprint::CollisionMask::PLAYER,
										.is_sensor = false,
										.enable_sensor_events = false,
										.enable_contact_events = true,
								},
								.shape =
								blueprint::CollisionShape::circle
								{
										.center = {.x = 0, .y = 0},
										.radius = 32,
								},
						},
						//
				},
		};

		return
		{
				.position = position,
				.sprite = std::move(sprite),
				.collision = std::move(collision),
				.property =
				{
						.health = 50,
						.mana = 20,
						.invincible = false,
						.infinity_mana = false,
				},
				.speed = 120,
		};
	}
}
