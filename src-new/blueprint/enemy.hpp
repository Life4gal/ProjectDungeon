// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/sprite_animation.hpp>
#include <blueprint/detail/physics.hpp>
#include <blueprint/detail/actor.hpp>
#include <blueprint/detail/ai.hpp>

namespace pd::blueprint
{
	// 敌人类型
	enum class EnemyType : std::uint8_t
	{
		RAT = 0,
		SLIME,
		BAT,
	};

	class Enemy
	{
	public:
		// 精灵动画
		SpriteAnimation animation;

		// 生成位置
		Position position;

		// 敌人类型
		EnemyType type;
		// AI
		Ai ai;
		// Actor
		Actor actor;

		// 接触伤害
		float contact_damage;

		// 任意形状物理体
		// TODO: 如果敌人存在多个碰撞体时如何处理?
		BodyDesc body_desc;
		ShapeDesc shape_desc;
		ShapeCategory::Any shape;
	};
}
