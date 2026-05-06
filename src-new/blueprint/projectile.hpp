// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

// #include <blueprint/detail/position.hpp>
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
		// position == owner.position
		// Position position;
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
