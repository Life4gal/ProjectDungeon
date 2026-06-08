// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/particle.hpp>

namespace pd::blueprint
{
	// 粒子发射器
	class ParticleEmitter final
	{
	public:
		// 定时 -> 移除发射器组件
		ScheduledTask::TimeCycle emitter;
		// 定时 -> 发射粒子
		ScheduledTask::TimeCycle emission;

		// 发射的粒子
		Particle particle;
	};
}
