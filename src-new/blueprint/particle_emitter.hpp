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
		// 总工作时间(毫秒)
		int working_time_ms;
		// 发射间隔(毫秒)
		int emission_interval_ms;

		// 发射的粒子
		Particle particle;
	};
}
