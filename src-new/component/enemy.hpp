// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/position.hpp>
#include <component/sprite_animation.hpp>
#include <component/physics_body.hpp>
#include <component/tags.hpp>
#include <component/actor.hpp>
#include <component/ai.hpp>

namespace pd::component::enemy
{
	// position::World --> 敌人绝对位置
	// position::Screen --> 敌人屏幕位置
	// sprite --> 敌人精灵
	// physics_body --> 敌人物理刚体
	// tags --> 敌人标签
	// actor --> 敌人Actor属性
	// AI --> 敌人AI属性

	// 物理碰撞体
	class PhysicsShape final
	{
	public:
		b2ShapeId shape;
	};

	// 接触伤害
	class ContactDamage final
	{
	public:
		float contact_damage;
	};

	// 敌人类型
	// 必须等价于blueprint::EnemyType
	enum class Type : std::uint8_t
	{
		RAT = 0,
		SLIME,
		BAT,
	};

	// ============================================

	// 敌人所属房间
	// 该组件不由factory::Enemy附加,而是由factory::Room附加
	class Room final
	{
	public:
		entt::entity room;
	};
}
