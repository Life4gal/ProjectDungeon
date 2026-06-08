// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/particle.hpp>

namespace pd::component::particle_emitter
{
	// 当前发射器是否处于冷却中
	// scheduled_task --> 移除该组件 --> 可以发射 --> 发射 --> 添加该组件
	class Cooldown {};

	// 粒子蓝图
	class Particle final
	{
	public:
		blueprint::Particle particle;
	};
}
