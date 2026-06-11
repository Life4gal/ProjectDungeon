// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/enemy.hpp>

#include <designer/character.hpp>

namespace pd::designer
{
	auto Enemy::rat() noexcept -> blueprint::Enemy
	{
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
				.type = blueprint::EnemyType::RAT,
				.character = Character::rat(),
				.ai = ai,
				.contact_damage = 20,
		};
	}

	auto Enemy::slime() noexcept -> blueprint::Enemy
	{
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
				.type = blueprint::EnemyType::SLIME,
				.character = Character::slime(),
				.ai = ai,
				.contact_damage = 15,
		};
	}

	auto Enemy::bat() noexcept -> blueprint::Enemy
	{
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
				.type = blueprint::EnemyType::BAT,
				.character = Character::bat(),
				.ai = ai,
				.contact_damage = 10,
		};
	}
}
