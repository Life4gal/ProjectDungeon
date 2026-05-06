// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/projectile.hpp>

namespace pd::designer
{
	auto Projectile::standard() noexcept -> blueprint::Projectile
	{
		blueprint::SpriteAnimation animation
		{
				.frames =
				{
						// 第一帧
						{.texture = "./assets/tileset/wall.png", .position = {.x = 0, .y = 0}},
						// 第二帧
						{.texture = "./assets/tileset/wall.png", .position = {.x = 64, .y = 0}},
						// 第三帧
						{.texture = "./assets/tileset/wall.png", .position = {.x = 128, .y = 0}},
						// 第四帧
						{.texture = "./assets/tileset/wall.png", .position = {.x = 192, .y = 0}},
				},
				.size = {.width = 32, .height = 32},
				.origin = {.x = 16, .y = 16},
				.scale = {.x = 1, .y = 1},
				.duration_ms = 100,
				.looping = true,
				.reversed = false,
		};
		constexpr blueprint::PhysicsBody physics_body{.type = blueprint::PhysicsBodyType::DYNAMIC, .fixed_rotation = true, .is_bullet = true};
		constexpr blueprint::PhysicsShapeCircle physics_shape
		{
				.def =
				{
						.material = {.friction = 0.3f, .restitution = 0},
						.density = 1,
						.category = blueprint::PhysicsShapeType::PROJECTILE,
						.category_mask = blueprint::PhysicsShapeCollisionMask::projectile,
						.is_sensor = false,
						.enable_sensor_events = true,
						.enable_contact_events = true,
				},
				.radius = 16,
		};

		return
		{
				.animation = std::move(animation),
				.damage = 10,
				.lifetime = 3,
				.speed = 200,
				.type = blueprint::ProjectileType::STANDARD,
				.physics_body = physics_body,
				.physics_shape = physics_shape,
		};
	}
}
