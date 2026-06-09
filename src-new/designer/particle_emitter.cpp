// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/particle_emitter.hpp>

#include <designer/particle.hpp>

namespace pd::designer
{
	auto ParticleEmitter::projectile_trail() noexcept -> blueprint::ParticleEmitter
	{
		return
		{
				.emitter = {.time_ms = 2050},
				.emission = {.time_ms = 200},
				.particle = Particle::projectile(),
		};
	}
}
