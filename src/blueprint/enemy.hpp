// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <config/enemy.hpp>

namespace pd::blueprint
{
	// TODO:
	//  1.Enemy使用字符串动画名,这里如果转换为动画ID则不需要每次获取动画都要计算一次ID
	//  2.Enemy使用字符串武器名,这里如果转换为武器ID则不需要每次获取武器都要计算一次ID

	// 一个敌人的数据(对应config::Enemy)
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
		config::Movement movement;
	};

	// 敌人集(对应config::EnemySet)
	// 敌人名称<->敌人数据
	class EnemySet final : public std::unordered_map<std::string, Enemy, utility::StringHash, std::ranges::equal_to> {};

	// 将config数据转换为蓝图数据
	[[nodiscard]] auto load_enemy(const config::EnemySet& data) noexcept -> EnemySet;
}
