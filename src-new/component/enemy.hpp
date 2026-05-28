// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/def.hpp>

#include <component/transform.hpp>
#include <component/render.hpp>
#include <component/collision.hpp>
#include <component/property.hpp>
#include <component/property_state.hpp>
#include <component/ai.hpp>
#include <component/tags.hpp>

namespace pd::component::enemy
{
	// transform --> 敌人变换
	// render --> 敌人渲染
	// collision --> 敌人碰撞体
	// property --> 敌人属性
	// property_state --> 敌人属性状态
	// AI --> 敌人AI
	// tags --> 敌人标签

	// 接触伤害
	class ContactDamage final
	{
	public:
		float contact_damage;
	};

	// 敌人类型
	using blueprint::EnemyType;

	// ============================================

	// 敌人所属房间
	// 该组件不由factory::Enemy附加,而是由factory::Room附加
	class Room final
	{
	public:
		entt::entity room;
	};
}
