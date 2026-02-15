// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <blueprint/enemy.hpp>

namespace pd::blueprint
{
	auto load_enemy(const config::EnemySet& data) noexcept -> EnemySet
	{
		EnemySet enemy_set{};

		enemy_set.reserve(data.size());

		for (auto&& [enemy_name, enemy_data]: data /* | std::views::as_rvalue*/)
		{
			Enemy enemy{};

			enemy.scale_x = enemy_data.scale_x;
			enemy.scale_y = enemy_data.scale_y;

			enemy.animation_name = enemy_data.animation_name;

			enemy.health = enemy_data.health;
			enemy.mana = enemy_data.mana;
			enemy.speed = enemy_data.speed;
			enemy.collision_damage = enemy_data.collision_damage;

			enemy.weapons = enemy_data.weapons;

			enemy.movement = enemy_data.movement;

			enemy_set.emplace(enemy_name, std::move(enemy));
		}

		return enemy_set;
	}
}
