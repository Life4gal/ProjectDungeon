// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <config/config_reader.hpp>
#include <config/set.hpp>

namespace pd::config
{
	class Enemy final
	{
	public:
		// 动画ID
		std::string animation_id;

		// 生命值
		float health;
		// 魔法值
		float mana;
		// 移动速度
		float speed;
		// 碰撞伤害(敌人触碰玩家造成的伤害)
		float collision_damage;

		// 武器的名称列表
		std::vector<std::string> weapons;
	};

	// 敌人集
	// 敌人ID -> 敌人配置
	// JSON:
	// { "enemies": "/path/to/enemies.json" }
	// { "enemies": { "enemy-id-1": { Enemy }, "enemy-id-2": { Enemy }, ... } }
	class EnemySet final : public Set<Enemy>
	{
	public:
		using Set::Set;
	};

	[[nodiscard]] auto load_enemy_from_json(Enemy& enemy, const ConfigReader::json_format& json) noexcept -> bool;
	[[nodiscard]] auto load_enemy_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<Enemy>;

	[[nodiscard]] auto load_enemy_set_from_json(EnemySet& enemy_set, const ConfigReader::json_format& json) noexcept -> bool;
	[[nodiscard]] auto load_enemy_set_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<EnemySet>;
}
