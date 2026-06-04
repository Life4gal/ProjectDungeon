// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/transform.hpp>
#include <component/render.hpp>
#include <component/collision.hpp>
#include <component/particle_effect.hpp>
#include <component/tags.hpp>

#include <SFML/System/Time.hpp>

namespace pd::component::projectile
{
	// transform --> 飞弹变换
	// render --> 飞弹渲染
	// collision --> 飞弹碰撞体
	// particle_effect --> 飞弹拖尾效果
	// tags --> 飞弹标签

	// 飞弹所有者
	class Owner final
	{
	public:
		entt::entity owner;
	};

	// 飞弹飞行速度(实际速度取决于物理体)
	class Speed final
	{
	public:
		float speed;
	};

	// 飞弹飞行生命周期
	class Lifetime final
	{
	public:
		sf::Time remaining;
	};

	// 飞弹伤害
	class Damage final
	{
	public:
		float damage;
	};
}
