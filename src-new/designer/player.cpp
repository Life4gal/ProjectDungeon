// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/player.hpp>

namespace pd::designer
{
	auto Player::test_character() noexcept -> blueprint::Player
	{
		// 渲染(必须是动态精灵)
		blueprint::Sprite::Dynamic sprite
		{
				.frames =
				{
						// 第一帧
						{.texture = "./assets/tileset/player.png", .uv_position = {.x = 0, .y = 0}, .uv_size = {.width = 64, .height = 64}, .pivot = {.x = 32, .y = 32}, .duration_ms = 250},
						// 第二帧
						{.texture = "./assets/tileset/player.png", .uv_position = {.x = 64, .y = 0}, .uv_size = {.width = 64, .height = 64}, .pivot = {.x = 32, .y = 32}, .duration_ms = 250},
						// 第三帧
						{.texture = "./assets/tileset/player.png", .uv_position = {.x = 128, .y = 0}, .uv_size = {.width = 64, .height = 64}, .pivot = {.x = 32, .y = 32}, .duration_ms = 250},
						// 第四帧
						{.texture = "./assets/tileset/player.png", .uv_position = {.x = 192, .y = 0}, .uv_size = {.width = 64, .height = 64}, .pivot = {.x = 32, .y = 32}, .duration_ms = 250},
				},
				.render_layer = blueprint::RenderLayer::PLAYER,
				.mode = blueprint::AnimationMode::LOOP,
				.direction = blueprint::AnimationDirection::FORWARD,
				.pause = false,
		};
		// 碰撞体
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
										.enable_sensor_events = true,
										.enable_contact_events = true,
								},
								.shape =
								blueprint::CollisionShape::Circle
								{
										.center = {.x = 0, .y = 0},
										.radius = 32,
								},
						},
						//
				},
		};
		// 属性
		constexpr blueprint::Property property
		{
				.health = 50,
				.mana = 20,
				.invincible = false,
				.infinity_mana = false,
		};

		return
		{
				.position = {.x = 0, .y = 0},
				.sprite = std::move(sprite),
				.collision = std::move(collision),
				.property = property,
				.name = {.type = blueprint::NameType::PLAYER_DEFAULT},
				.speed = 60 * 4.0f,
		};
	}
}
