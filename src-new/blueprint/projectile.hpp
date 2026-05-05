// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/transform.hpp>
#include <blueprint/detail/sprite_animation.hpp>
#include <blueprint/detail/physics_body.hpp>
#include <blueprint/detail/physics_shape.hpp>

namespace pd::blueprint
{
	// 飞弹类型
	enum class ProjectileType : std::uint8_t
	{
		STANDARD = 0,
	};

	class Projectile final
	{
	public:
		// TODO: 这里的transform起一个什么作用?
		//  Transform::x/y作为基础偏移,然后实际位置=偏移+发射者位置?
		//  如果是偏移还要考虑朝向
		Transform transform;
		SpriteAnimation animation;

		float damage;
		float lifetime;
		float speed;
		ProjectileType type;

		// 物理刚体
		PhysicsBody physics_body;
		// 圆形碰撞体
		PhysicsShapeCircle physics_shape;
	};
}
