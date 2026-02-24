// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include <config/types.hpp>

#include <utility/string_hash.hpp>

namespace pd::config
{
	// 一个敌人的数据
	class Enemy final
	{
	public:
		// 尺寸(比例)
		float scale_x;
		float scale_y;

		// 动画名称
		std::string animation_name;

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

		// 移动类型
		Movement movement;
	};

	// 敌人集
	// 敌人名称<->敌人数据
	class EnemySet final : public std::unordered_map<
				std::string,
				Enemy,
				utility::StringHash,
				std::ranges::equal_to
			> {};

	// 从文件中读取敌人数据
	[[nodiscard]] auto load_enemy(const std::filesystem::path& path) noexcept -> EnemySet;
} // namespace pd::config
