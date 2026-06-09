// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/projectile.hpp>

#include <designer/particle_emitter.hpp>

namespace pd::designer
{
	auto Projectile::standard() noexcept -> blueprint::Projectile
	{
		// 渲染(必须是动态精灵)
		blueprint::Sprite::Dynamic sprite
		{
				.frames =
				{
						// 第一帧
						{.texture = "./assets/projectile_standard.png", .uv_position = {.x = 0, .y = 0}, .uv_size = {.width = 24, .height = 24}, .pivot = {.x = 12, .y = 12}, .duration_ms = 1000},
				},
				.render_layer = blueprint::RenderLayer::PROJECTILE,
				.looping = true,
				.reversed = false,
				.pause = false,
		};
		// 碰撞体
		blueprint::Collision collision
		{
				.def =
				{
						.type = blueprint::CollisionBodyType::DYNAMIC,
						.fixed_rotation = true,
						.is_bullet = true,
				},
				.shapes =
				{
						// 圆形碰撞体
						{
								.def =
								{
										.material = {.friction = 0.3f, .restitution = 0},
										.density = 1,
										.category = blueprint::CollisionCategory::PROJECTILE,
										.mask = blueprint::CollisionMask::PROJECTILE,
										.is_sensor = false,
										.enable_sensor_events = true,
										.enable_contact_events = true,
								},
								.shape =
								blueprint::CollisionShape::Circle
								{
										.center = {.x = 0, .y = 0},
										.radius = 12,
								},
						},
						//
				},
		};
		// 弹道
		constexpr blueprint::Trajectory trajectory
		{
				blueprint::Trajectory::Straight
				{
						.speed = 60 * 12.0f,
				},
		};
		// 拖尾效果
		auto particle_emitter = ParticleEmitter::projectile_trail();

		return
		{
				.sprite = std::move(sprite),
				.collision = std::move(collision),
				.trajectory = trajectory,
				.particle_emitter = std::move(particle_emitter),
				.lifetime = {.time_ms = 3000},
				.damage = 10,
		};
	}
}
