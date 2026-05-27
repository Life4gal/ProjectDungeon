// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/projectile.hpp>

namespace pd::designer
{
	auto Projectile::standard() noexcept -> blueprint::Projectile
	{
		blueprint::DynamicSprite sprite
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
				.duration_ms = 100,
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
										.radius = 16,
								},
								blueprint::CollisionShapeDef
								{
										.material = {.friction = 0.3f, .restitution = 0},
										.density = 1,
										.category = blueprint::CollisionCategory::PROJECTILE,
										.mask = blueprint::CollisionMask::PROJECTILE,
										.is_sensor = false,
										.enable_sensor_events = true,
										.enable_contact_events = true,
								},
						},
						//
				},
		};
		constexpr blueprint::Trajectory trajectory
		{
				blueprint::Trajectory::straight
				{
						.speed = 200,
				},
		};

		return
		{
				.sprite = std::move(sprite),
				.collision = std::move(collision),
				.trajectory = trajectory,
				.lifetime = 3,
				.damage = 10,
		};
	}
}
