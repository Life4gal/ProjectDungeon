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
		const blueprint::Transform transform
		{
				.x = static_cast<float>(Room::tile_origin_x + tile_x * Room::tile_width),
				.y = static_cast<float>(Room::tile_origin_y + tile_y * Room::tile_height),
				.scale_x = 1,
				.scale_y = 1,
				.rotation = 0,
		};
		blueprint::SpriteAnimation animation
		{
				.frames =
				{
						// 第一帧
						{.texture = "./assets/tileset/rat.png", .x = 0, .y = 0},
						// 第二帧
						{.texture = "./assets/tileset/rat.png", .x = 64, .y = 0},
						// 第三帧
						{.texture = "./assets/tileset/rat.png", .x = 128, .y = 0},
						// 第四帧
						{.texture = "./assets/tileset/rat.png", .x = 192, .y = 0},
				},
				.width = 64,
				.height = 64,
				.origin_x = 32,
				.origin_y = 32,
				.duration_ms = 350,
				.looping = true,
				.reversed = false,
		};
		constexpr blueprint::PhysicsBody physics_body{.type = blueprint::PhysicsBodyType::DYNAMIC, .fixed_rotation = true, .is_bullet = false};
		constexpr blueprint::PhysicsShapeBox physics_shape
		{
				.def =
				{
						.material = {.friction = 0.3f, .restitution = 0},
						.density = 1,
						.category = blueprint::PhysicsShapeType::ENEMY,
						.category_mask = blueprint::PhysicsShapeCollisionMask::enemy,
						.is_sensor = false,
						.enable_sensor_events = false,
						.enable_contact_events = true,
				},
				.width = 64,
				.height = 64,
		};
		constexpr blueprint::Actor actor{.health = 60, .mana = 0, .speed = 60};

		return
		{
				.transform = transform,
				.animation = std::move(animation),
				.actor = actor,
				.contact_damage = 20,
				.type = blueprint::EnemyType::RAT,
				.ai = {.move_behavior = blueprint::MoveBehavior::WANDER},
				.physics_body = physics_body,
				.physics_shape = physics_shape,
		};
	}

	auto Enemy::slime(const size_type tile_x, const size_type tile_y) noexcept -> blueprint::Enemy
	{
		const blueprint::Transform transform
		{
				.x = static_cast<float>(Room::tile_origin_x + tile_x * Room::tile_width),
				.y = static_cast<float>(Room::tile_origin_y + tile_y * Room::tile_height),
				.scale_x = 1,
				.scale_y = 1,
				.rotation = 0,
		};
		blueprint::SpriteAnimation animation
		{
				.frames =
				{
						// 第一帧
						{.texture = "./assets/tileset/slime.png", .x = 0, .y = 0},
						// 第二帧
						{.texture = "./assets/tileset/slime.png", .x = 64, .y = 0},
						// 第三帧
						{.texture = "./assets/tileset/slime.png", .x = 128, .y = 0},
						// 第四帧
						{.texture = "./assets/tileset/slime.png", .x = 192, .y = 0},
				},
				.width = 64,
				.height = 64,
				.origin_x = 32,
				.origin_y = 32,
				.duration_ms = 200,
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
						.category = blueprint::PhysicsShapeType::ENEMY,
						.category_mask = blueprint::PhysicsShapeCollisionMask::enemy,
						.is_sensor = false,
						.enable_sensor_events = false,
						.enable_contact_events = true,
				},
				.radius = 32,
		};
		constexpr blueprint::Actor actor{.health = 80, .mana = 0, .speed = 40};

		return
		{
				.transform = transform,
				.animation = std::move(animation),
				.actor = actor,
				.contact_damage = 15,
				.type = blueprint::EnemyType::SLIME,
				.ai = {.move_behavior = blueprint::MoveBehavior::JUMP},
				.physics_body = physics_body,
				.physics_shape = physics_shape,
		};
	}

	auto Enemy::bat(const size_type tile_x, const size_type tile_y) noexcept -> blueprint::Enemy
	{
		const blueprint::Transform transform
		{
				.x = static_cast<float>(Room::tile_origin_x + tile_x * Room::tile_width),
				.y = static_cast<float>(Room::tile_origin_y + tile_y * Room::tile_height),
				.scale_x = 1,
				.scale_y = 1,
				.rotation = 0,
		};
		blueprint::SpriteAnimation animation
		{
				.frames =
				{
						// 第一帧
						{.texture = "./assets/tileset/bat.png", .x = 0, .y = 0},
						// 第二帧
						{.texture = "./assets/tileset/bat.png", .x = 64, .y = 0},
						// 第三帧
						{.texture = "./assets/tileset/bat.png", .x = 128, .y = 0},
						// 第四帧
						{.texture = "./assets/tileset/bat.png", .x = 192, .y = 0},
				},
				.width = 64,
				.height = 64,
				.origin_x = 32,
				.origin_y = 32,
				.duration_ms = 300,
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
						.category = blueprint::PhysicsShapeType::ENEMY,
						.category_mask = blueprint::PhysicsShapeCollisionMask::enemy,
						.is_sensor = false,
						.enable_sensor_events = false,
						.enable_contact_events = true,
				},
				.radius = 32,
		};
		constexpr blueprint::Actor actor{.health = 35, .mana = 0, .speed = 80};

		return
		{
				.transform = transform,
				.animation = std::move(animation),
				.actor = actor,
				.contact_damage = 10,
				.type = blueprint::EnemyType::BAT,
				.ai = {.move_behavior = blueprint::MoveBehavior::CHASE},
				.physics_body = physics_body,
				.physics_shape = physics_shape,
		};
	}
}
