// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/character.hpp>

#include <designer/animation.hpp>

namespace pd::designer
{
	auto Character::rat() noexcept -> blueprint::Character
	{
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

		return
		{
				.position = {.x = 0, .y = 0},
				.animation = Animation::rat(),
				.collision = std::move(collision),
				.property = property,
				.name = {.type = blueprint::NameType::ENEMY_RAT},
		};
	}

	auto Character::slime() noexcept -> blueprint::Character
	{
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

		return
		{
				.position = {.x = 0, .y = 0},
				.animation = Animation::slime(),
				.collision = std::move(collision),
				.property = property,
				.name = {.type = blueprint::NameType::ENEMY_SLIME},
		};
	}

	auto Character::bat() noexcept -> blueprint::Character
	{
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

		return
		{
				.position = {.x = 0, .y = 0},
				.animation = Animation::bat(),
				.collision = std::move(collision),
				.property = property,
				.name = {.type = blueprint::NameType::ENEMY_BAT},
		};
	}

	auto Character::player_default() noexcept -> blueprint::Character
	{
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
				.animation = Animation::player_default(),
				.collision = std::move(collision),
				.property = property,
				.name = {.type = blueprint::NameType::PLAYER_DEFAULT},
		};
	}
}
