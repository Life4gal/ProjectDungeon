// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/enemy.hpp>

namespace pd::designer
{
	auto Enemy::rat() noexcept -> blueprint::Enemy
	{
		// 渲染(必须是动态精灵)
		blueprint::Sprite::Dynamic sprite
		{
				.frames =
				{
						// 下 60*136
						// 第一帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 30, .y = 16}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 第二帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 286, .y = 16}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 第三帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 540, .y = 16}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 第四帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 800, .y = 16}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 上 60*136
						// 第一帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 30, .y = 270}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 第二帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 286, .y = 270}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 第三帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 540, .y = 270}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 第四帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 800, .y = 270}, .uv_size = {.width = 60, .height = 136}, .pivot = {.x = 30, .y = 58}, .duration_ms = 350},
						// 左 140*116
						// 第一帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 6, .y = 524}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 50, .y = 56}, .duration_ms = 350},
						// 第二帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 262, .y = 524}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 50, .y = 56}, .duration_ms = 350},
						// 第三帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 520, .y = 524}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 50, .y = 56}, .duration_ms = 350},
						// 第四帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 774, .y = 524}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 50, .y = 56}, .duration_ms = 350},
						// 右 140*116
						// 第一帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 44, .y = 780}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 90, .y = 56}, .duration_ms = 350},
						// 第二帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 282, .y = 780}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 90, .y = 56}, .duration_ms = 350},
						// 第三帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 556, .y = 780}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 90, .y = 56}, .duration_ms = 350},
						// 第四帧
						{.texture = "./assets/rat.png", .uv_position = {.x = 808, .y = 780}, .uv_size = {.width = 140, .height = 116}, .pivot = {.x = 90, .y = 56}, .duration_ms = 350},
				},
				.render_layer = blueprint::RenderLayer::ENEMY,
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
						// 矩形碰撞体
						{
								.def =
								{
										.material = {.friction = 0.3f, .restitution = 0},
										.density = 1,
										.category = blueprint::CollisionCategory::ENEMY,
										.mask = blueprint::CollisionMask::ENEMY,
										.is_sensor = false,
										.enable_sensor_events = false,
										.enable_contact_events = true,
								},
								.shape =
								blueprint::CollisionShape::Box
								{
										.size = {.width = 60, .height = 120},
								},
						},
						// 
				},
		};
		// 属性
		constexpr blueprint::Property property
		{
				.health = 60,
				.mana = 0,
				.invincible = false,
				.infinity_mana = false,
		};
		// AI
		constexpr blueprint::Ai ai
		{
				.move_behavior =
				{
						.behavior =
						blueprint::MoveBehavior::Wander
						{
								.speed = 60 * 2.0f,
								.next_turn_min_time = 1.5f,
								.next_turn_max_timer = 4.0f,
						},
				},
		};

		return
		{
				.position = {.x = 0, .y = 0},
				.sprite = std::move(sprite),
				.collision = std::move(collision),
				.type = blueprint::EnemyType::RAT,
				.property = property,
				.ai = ai,
				.name = {.type = blueprint::NameType::ENEMY_RAT},
				.contact_damage = 20,
		};
	}

	auto Enemy::slime() noexcept -> blueprint::Enemy
	{
		// 渲染(必须是动态精灵)
		blueprint::Sprite::Dynamic sprite
		{
				.frames =
				{
						// 下 64*80
						// 第一帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 28, .y = 42}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第二帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 286, .y = 42}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第三帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 542, .y = 42}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第四帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 796, .y = 46}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 上 64*80
						// 第一帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 28, .y = 300}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第二帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 286, .y = 300}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第三帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 542, .y = 300}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第四帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 796, .y = 300}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 左 64*80
						// 第一帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 28, .y = 558}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第二帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 286, .y = 558}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第三帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 542, .y = 558}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第四帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 796, .y = 558}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 右 64*80
						// 第一帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 28, .y = 812}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第二帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 286, .y = 812}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第三帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 542, .y = 812}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
						// 第四帧
						{.texture = "./assets/slime.png", .uv_position = {.x = 796, .y = 812}, .uv_size = {.width = 64, .height = 80}, .pivot = {.x = 32, .y = 32}, .duration_ms = 200},
				},
				.render_layer = blueprint::RenderLayer::ENEMY,
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
										.category = blueprint::CollisionCategory::ENEMY,
										.mask = blueprint::CollisionMask::ENEMY,
										.is_sensor = false,
										.enable_sensor_events = false,
										.enable_contact_events = true,
								},
								.shape =
								blueprint::CollisionShape::Circle
								{
										.center = {.x = 0, .y = 8},
										.radius = 32,
								},
						},
						//
				},
		};
		// 属性
		constexpr blueprint::Property property
		{
				.health = 80,
				.mana = 0,
				.invincible = false,
				.infinity_mana = false,
		};
		// AI
		constexpr blueprint::Ai ai
		{
				.move_behavior =
				{
						.behavior =
						blueprint::MoveBehavior::ChaseJump
						{
								.speed = 60 * 6.5f,
								.duration = 0.6f,
								.next_jump_min_time = 0.8f,
								.next_jump_max_time = 1.6f,
						},
				},
		};

		return
		{
				.position = {.x = 0, .y = 0},
				.sprite = std::move(sprite),
				.collision = std::move(collision),
				.type = blueprint::EnemyType::SLIME,
				.property = property,
				.ai = ai,
				.name = {.type = blueprint::NameType::ENEMY_SLIME},
				.contact_damage = 15,
		};
	}

	auto Enemy::bat() noexcept -> blueprint::Enemy
	{
		// 渲染(必须是动态精灵)
		blueprint::Sprite::Dynamic sprite
		{
				.frames =
				{
						// 下 100*100
						// 第一帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 12, .y = 20}, .uv_size = {.width = 100, .height = 100}, .pivot = {.x = 50, .y = 50}, .duration_ms = 300},
						// 第二帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 268, .y = 20}, .uv_size = {.width = 100, .height = 100}, .pivot = {.x = 50, .y = 50}, .duration_ms = 300},
						// 第三帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 524, .y = 20}, .uv_size = {.width = 100, .height = 100}, .pivot = {.x = 50, .y = 50}, .duration_ms = 300},
						// 第四帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 780, .y = 20}, .uv_size = {.width = 100, .height = 100}, .pivot = {.x = 50, .y = 50}, .duration_ms = 300},
						// 上 120*100
						// 第一帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 2, .y = 272}, .uv_size = {.width = 120, .height = 100}, .pivot = {.x = 60, .y = 50}, .duration_ms = 300},
						// 第二帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 256, .y = 272}, .uv_size = {.width = 120, .height = 100}, .pivot = {.x = 60, .y = 50}, .duration_ms = 300},
						// 第三帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 510, .y = 272}, .uv_size = {.width = 120, .height = 100}, .pivot = {.x = 60, .y = 50}, .duration_ms = 300},
						// 第四帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 770, .y = 272}, .uv_size = {.width = 120, .height = 100}, .pivot = {.x = 60, .y = 50}, .duration_ms = 300},
						// 左 150*100
						// 第一帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 0, .y = 530}, .uv_size = {.width = 150, .height = 100}, .pivot = {.x = 75, .y = 50}, .duration_ms = 300},
						// 第二帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 242, .y = 530}, .uv_size = {.width = 150, .height = 100}, .pivot = {.x = 75, .y = 50}, .duration_ms = 300},
						// 第三帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 500, .y = 530}, .uv_size = {.width = 150, .height = 100}, .pivot = {.x = 75, .y = 50}, .duration_ms = 300},
						// 第四帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 754, .y = 530}, .uv_size = {.width = 150, .height = 100}, .pivot = {.x = 75, .y = 50}, .duration_ms = 300},
						// 右 136*110
						// 第一帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 0, .y = 780}, .uv_size = {.width = 136, .height = 110}, .pivot = {.x = 68, .y = 55}, .duration_ms = 300},
						// 第二帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 250, .y = 780}, .uv_size = {.width = 136, .height = 110}, .pivot = {.x = 68, .y = 55}, .duration_ms = 300},
						// 第三帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 508, .y = 780}, .uv_size = {.width = 136, .height = 110}, .pivot = {.x = 68, .y = 55}, .duration_ms = 300},
						// 第四帧
						{.texture = "./assets/bat.png", .uv_position = {.x = 760, .y = 780}, .uv_size = {.width = 136, .height = 110}, .pivot = {.x = 68, .y = 55}, .duration_ms = 300},
				},
				.render_layer = blueprint::RenderLayer::ENEMY,
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
						// 胶囊形碰撞体
						{
								.def =
								{
										.material = {.friction = 0.3f, .restitution = 0},
										.density = 1,
										.category = blueprint::CollisionCategory::ENEMY,
										.mask = blueprint::CollisionMask::ENEMY,
										.is_sensor = false,
										.enable_sensor_events = false,
										.enable_contact_events = true,
								},
								.shape =
								blueprint::CollisionShape::Capsule
								{
										.center1 = {.x = -35, .y = 0},
										.center2 = {.x = 35, .y = 0},
										.radius = 35,
								},
						},
						//
				},
		};
		// 属性
		constexpr blueprint::Property property
		{
				.health = 35,
				.mana = 0,
				.invincible = false,
				.infinity_mana = false,
		};
		// AI
		constexpr blueprint::Ai ai
		{
				.move_behavior =
				{
						.behavior =
						blueprint::MoveBehavior::Chase
						{
								.speed = 60 * 2.6667f,
						},
				},
		};

		return
		{
				.position = {.x = 0, .y = 0},
				.sprite = std::move(sprite),
				.collision = std::move(collision),
				.type = blueprint::EnemyType::BAT,
				.property = property,
				.ai = ai,
				.name = {.type = blueprint::NameType::ENEMY_BAT},
				.contact_damage = 10,
		};
	}
}
