// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/particle_effect.hpp>

#include <component/particle_effect.hpp>
#include <component/transform.hpp>

#include <entt/entt.hpp>
#include <utility>

namespace pd::update
{
	using namespace component;

	auto particle_effect(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		const auto view = registry
				.view<
					const particle_effect::TotalWorkingTime,
					particle_effect::WorkingTime,
					const particle_effect::EmissionInterval,
					particle_effect::EmissionCooldown,
					const particle_effect::ParticleLifetime,
					const particle_effect::ParticleAcceleration,
					particle_effect::Particles,
					transform::Position
				>();

		for (const auto [entity,total_working_time, working_time, emission_interval, emission_cooldown, particle_lifetime, particle_acceleration, particles, position]: view.each())
		{
			working_time.time += delta;

			// 仅在工作时间小于总工作时间时才发射粒子
			if (working_time.time < total_working_time.time)
			{
				emission_cooldown.cooldown -= delta;

				// 仅在冷却完毕时才发射粒子
				if (emission_cooldown.cooldown <= sf::Time::Zero)
				{
					particles.particles.push_back(
						{
								.position = position.position,
								.velocity = {0, 0},
								.elapsed = sf::Time::Zero
						}
					);
					emission_cooldown.cooldown += emission_interval.interval;
				}
			}

			// 更新粒子
			for (const auto delta_seconds = delta.asSeconds();
			     auto& particle: particles.particles)
			{
				particle.velocity += particle_acceleration.acceleration * delta_seconds;
				particle.position += particle.velocity * delta_seconds;
				particle.elapsed += delta;
			}

			// 淘汰已过期粒子
			for (std::size_t i = 0; i < particles.particles.size();)
			{
				if (auto& particle = particles.particles[i];
					particle.elapsed >= particle_lifetime.lifetime)
				{
					// SWAP AND POP
					particle = particles.particles.back();
					particles.particles.pop_back();
				}
				else
				{
					i += 1;
				}
			}
		}
	}
}
