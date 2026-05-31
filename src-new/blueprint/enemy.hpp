// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/render.hpp>
#include <blueprint/detail/collision.hpp>
#include <blueprint/detail/property.hpp>
#include <blueprint/detail/ai.hpp>

namespace pd::blueprint
{
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
		// TODO: 安装CLAW,使用CLAW攻击造成伤害,而不是基于接触造成伤害
		float contact_damage;

		//
	};
}
