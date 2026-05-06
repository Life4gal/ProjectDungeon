// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/position.hpp>
#include <component/sprite_animation.hpp>
#include <component/physics_body.hpp>
#include <component/tags.hpp>

#include <SFML/System/Time.hpp>

namespace pd::component::projectile
{
	// position::World --> 飞弹绝对位置
	// position::Screen --> 飞弹屏幕位置
	// sprite_animation --> 飞弹精灵动画
	// physics_body --> 飞弹物理刚体
	// tags --> 飞弹标签

	// 物理碰撞体
	class PhysicsShape final
	{
	public:
		b2ShapeId shape;
	};

	// 飞弹类型
	// 必须等价于blueprint::ProjectileType
	enum class Type : std::uint8_t
	{
		STANDARD = 0,
	};

	// 飞弹所有者
	class Owner final
	{
	public:
		entt::entity owner;
	};

	// 飞弹伤害
	class Damage final
	{
	public:
		float damage;
	};

	// 飞弹飞行生命周期
	class Lifetime final
	{
	public:
		sf::Time remaining;
	};

	// 飞弹飞行速度(实际速度取决于物理体)
	class Speed final
	{
	public:
		float speed;
	};
}
