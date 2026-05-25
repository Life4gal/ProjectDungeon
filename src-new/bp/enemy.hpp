// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <bp/detail/render.hpp>
#include <bp/detail/collision.hpp>
#include <bp/detail/property.hpp>
#include <bp/detail/ai.hpp>

namespace pd::bp
{
	// 敌人类型
	enum class EnemyType : std::uint8_t
	{
		// 老鼠
		RAT = 0,
		// 史莱姆
		SLIME,
		// 蝙蝠
		BAT,
	};

	// 敌人
	class Enemy final
	{
	public:
		// 敌人的初始位置
		Position position;

		// 渲染(必须是动态精灵)
		DynamicSprite sprite;

		// 碰撞体
		Collision collision;

		// 敌人类型
		EnemyType type;
		// 敌人属性
		Property property;
		// 敌人AI
		Ai ai;

		// 接触伤害
		float contact_damage;

		//
	};
}
