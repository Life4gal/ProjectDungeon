// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <variant>

#include <blueprint/detail/position.hpp>
#include <blueprint/detail/sprite_animation.hpp>
#include <blueprint/detail/physics_body.hpp>
#include <blueprint/detail/physics_shape.hpp>
#include <blueprint/detail/actor.hpp>
#include <blueprint/detail/ai.hpp>

namespace pd::blueprint
{
	enum class EnemyType : std::uint8_t
	{
		RAT = 0,
		SLIME,
		BAT,
	};

	class Enemy
	{
	public:
		using physics_shape_type = std::variant<PhysicsShapeCircle, PhysicsShapeCapsule, PhysicsShapeBox>;

		Position position;
		SpriteAnimation animation;

		Actor actor;

		// 接触伤害
		float contact_damage;

		EnemyType type;
		Ai ai;

		// 物理刚体
		PhysicsBody physics_body;
		// 物理碰撞体
		// TODO: 如果敌人存在多个碰撞体时如何处理?
		physics_shape_type physics_shape;
	};
}
