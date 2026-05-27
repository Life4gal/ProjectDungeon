// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/enemy.hpp>

#include <designer/room.hpp>

namespace pd::designer
{
	auto Enemy::rat(const size_type tile_x, const size_type tile_y) noexcept -> blueprint::Enemy
	{
		const blueprint::Position position
		{
				.x = static_cast<float>(Room::tile_origin_x + tile_x * Room::tile_width),
				.y = static_cast<float>(Room::tile_origin_y + tile_y * Room::tile_height),
		};
		blueprint::DynamicSprite sprite
		{
				.frames =
				{
						// 第一帧
						{.texture = "./assets/tileset/rat.png", .position = {.x = 0, .y = 0}},
						// 第二帧
						{.texture = "./assets/tileset/rat.png", .position = {.x = 64, .y = 0}},
						// 第三帧
						{.texture = "./assets/tileset/rat.png", .position = {.x = 128, .y = 0}},
						// 第四帧
						{.texture = "./assets/tileset/rat.png", .position = {.x = 192, .y = 0}},
				},
				.size = {.width = 64, .height = 64},
				.origin = {.x = 32, .y = 32},
				.duration_ms = 350,
				.looping = true,
				.reversed = false,
		};
		blueprint::Collision collision
		{
				.type = blueprint::CollisionBodyType::DYNAMIC,
				.shapes =
				{
						// 矩形碰撞体
						{
								blueprint::CollisionShape::box
								{
										.size = {.width = 64, .height = 64},
								},
								blueprint::CollisionShapeDef
								{
										.material = {.friction = 0.3f, .restitution = 0},
										.density = 1,
										.category = blueprint::CollisionCategory::ENEMY,
										.mask = blueprint::CollisionMask::ENEMY,
										.is_sensor = false,
										.enable_sensor_events = false,
										.enable_contact_events = true,
								},
						},
						// 
				},
		};
		constexpr blueprint::Property property
		{
				.health = 60,
				.mana = 0,
				.invincible = false,
				.infinity_mana = false,
		};
		constexpr blueprint::Ai ai
		{
				.move_behavior =
				blueprint::MoveBehavior::wander
				{
						.speed = 60,
						.next_turn_min_time = 1.5f,
						.next_turn_max_timer = 4.0f,
				},
		};

		return
		{
				.position = position,
				.sprite = std::move(sprite),
				.collision = std::move(collision),
				.type = blueprint::EnemyType::RAT,
				.property = property,
				.ai = ai,
				.contact_damage = 20,
		};
	}

	auto Enemy::slime(const size_type tile_x, const size_type tile_y) noexcept -> blueprint::Enemy
	{
		const blueprint::Position position
		{
				.x = static_cast<float>(Room::tile_origin_x + tile_x * Room::tile_width),
				.y = static_cast<float>(Room::tile_origin_y + tile_y * Room::tile_height),
		};
		blueprint::DynamicSprite sprite
		{
				.frames =
				{
						// 第一帧
						{.texture = "./assets/tileset/slime.png", .position = {.x = 0, .y = 0}},
						// 第二帧
						{.texture = "./assets/tileset/slime.png", .position = {.x = 64, .y = 0}},
						// 第三帧
						{.texture = "./assets/tileset/slime.png", .position = {.x = 128, .y = 0}},
						// 第四帧
						{.texture = "./assets/tileset/slime.png", .position = {.x = 192, .y = 0}},
				},
				.size = {.width = 64, .height = 64},
				.origin = {.x = 32, .y = 32},
				.duration_ms = 200,
				.looping = true,
				.reversed = false,
		};
		blueprint::Collision collision
		{
				.type = blueprint::CollisionBodyType::DYNAMIC,
				.shapes =
				{
						// 圆形碰撞体
						{
								blueprint::CollisionShape::circle
								{
										.center = {.x = 0, .y = 0},
										.radius = 32,
								},
								blueprint::CollisionShapeDef
								{
										.material = {.friction = 0.3f, .restitution = 0},
										.density = 1,
										.category = blueprint::CollisionCategory::ENEMY,
										.mask = blueprint::CollisionMask::ENEMY,
										.is_sensor = false,
										.enable_sensor_events = false,
										.enable_contact_events = true,
								},
						},
						//
				},
		};
		constexpr blueprint::Property property
		{
				.health = 80,
				.mana = 0,
				.invincible = false,
				.infinity_mana = false,
		};
		constexpr blueprint::Ai ai
		{
				.move_behavior =
				blueprint::MoveBehavior::chase_jump
				{
						.speed = 200,
						.duration = 0.6f,
						.next_jump_min_time = 0.8f,
						.next_jump_max_time = 1.6f,
				},
		};

		return
		{
				.position = position,
				.sprite = std::move(sprite),
				.collision = std::move(collision),
				.type = blueprint::EnemyType::SLIME,
				.property = property,
				.ai = ai,
				.contact_damage = 15,
		};
	}

	auto Enemy::bat(const size_type tile_x, const size_type tile_y) noexcept -> blueprint::Enemy
	{
		const blueprint::Position position
		{
				.x = static_cast<float>(Room::tile_origin_x + tile_x * Room::tile_width),
				.y = static_cast<float>(Room::tile_origin_y + tile_y * Room::tile_height),
		};
		blueprint::DynamicSprite sprite
		{
				.frames =
				{
						// 第一帧
						{.texture = "./assets/tileset/bat.png", .position = {.x = 0, .y = 0}},
						// 第二帧
						{.texture = "./assets/tileset/bat.png", .position = {.x = 64, .y = 0}},
						// 第三帧
						{.texture = "./assets/tileset/bat.png", .position = {.x = 128, .y = 0}},
						// 第四帧
						{.texture = "./assets/tileset/bat.png", .position = {.x = 192, .y = 0}},
				},
				.size = {.width = 64, .height = 64},
				.origin = {.x = 32, .y = 32},
				.duration_ms = 300,
				.looping = true,
				.reversed = false,
		};
		blueprint::Collision collision
		{
				.type = blueprint::CollisionBodyType::DYNAMIC,
				.shapes =
				{
						// 圆形碰撞体
						{
								blueprint::CollisionShape::circle
								{
										.center = {.x = 0, .y = 0},
										.radius = 32,
								},
								blueprint::CollisionShapeDef
								{
										.material = {.friction = 0.3f, .restitution = 0},
										.density = 1,
										.category = blueprint::CollisionCategory::ENEMY,
										.mask = blueprint::CollisionMask::ENEMY,
										.is_sensor = false,
										.enable_sensor_events = false,
										.enable_contact_events = true,
								},
						},
						//
				},
		};
		constexpr blueprint::Property property
		{
				.health = 35,
				.mana = 0,
				.invincible = false,
				.infinity_mana = false,
		};
		constexpr blueprint::Ai ai
		{
				.move_behavior =
				blueprint::MoveBehavior::chase
				{
						.speed = 80,
				},
		};

		return
		{
				.position = position,
				.sprite = std::move(sprite),
				.collision = std::move(collision),
				.type = blueprint::EnemyType::BAT,
				.property = property,
				.ai = ai,
				.contact_damage = 10,
		};
	}
}
