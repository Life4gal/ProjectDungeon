// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/scheduled_task.hpp>
#include <blueprint/detail/render.hpp>
#include <blueprint/detail/collision.hpp>

#include <blueprint/particle_emitter.hpp>

namespace pd::blueprint
{
	// 飞行弹道
	class Trajectory final
	{
	public:
		// 直线弹道
		class Straight final
		{
		public:
			// 飞行速度
			// 飞行速度(velocity: Vec2)取决于发射方向
			float speed;
		};

		using trajectory_type = std::variant<
			Straight
		>;

		trajectory_type trajectory;
	};

	// 飞弹
	class Projectile final
	{
	public:
		// 初始位置取决于发射者

		// 渲染(必须是动态精灵)
		Sprite::Dynamic sprite;

		// 碰撞体
		Collision collision;

		// 飞行弹道
		Trajectory trajectory;
		// 粒子发射器(拖尾效果)
		ParticleEmitter particle_emitter;
		// 定时 -> 销毁
		ScheduledTask::TimeCycle lifetime;

		// 命中伤害
		// TODO: AOE? DOT?
		float damage;
	};
}
