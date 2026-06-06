// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/particle_emitter.hpp>

#include <component/particle_emitter.hpp>

#include <entt/entt.hpp>

namespace pd::factory::detail
{
	using namespace component;

	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::ParticleEmitter& particle_emitter) noexcept -> void
	{
		// working time
		registry.emplace<particle_emitter::TotalWorkingTime>(entity, sf::milliseconds(particle_emitter.working_time_ms));
		registry.emplace<particle_emitter::WorkingTime>(entity, sf::Time::Zero);
		// emission
		registry.emplace<particle_emitter::EmissionInterval>(entity, sf::milliseconds(particle_emitter.emission_interval_ms));
		registry.emplace<particle_emitter::EmissionCooldown>(entity, sf::Time::Zero);
		// particle
		registry.emplace<particle_emitter::Particle>(entity, particle_emitter.particle);
	}
}
