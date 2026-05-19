// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

// #include <blueprint/detail/position.hpp>
#include <blueprint/detail/sprite_animation.hpp>
#include <blueprint/detail/physics.hpp>

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
		// 精灵动画
		SpriteAnimation animation;

		// 生成位置
		// 生成位置取决于发射位置
		// Position position;

		// 攻击伤害
		float damage;
		// 最大飞行时长
		float lifetime;
		// 飞行速度
		float speed;
		// 飞弹类型
		ProjectileType type;

		// 圆形物理体
		BodyDesc body_desc;
		ShapeDesc shape_desc;
		ShapeCategory::Circle shape;
	};
}
