// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/particle_emitter.hpp>

// 创建粒子实体
#include <factory/particle.hpp>

#include <component/particle_emitter.hpp>
#include <component/transform.hpp>

#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;

	auto particle_emitter(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		const auto view = registry
				.view<
					const particle_emitter::TotalWorkingTime,
					particle_emitter::WorkingTime,
					const particle_emitter::EmissionInterval,
					particle_emitter::EmissionCooldown,
					const particle_emitter::Particle,
					transform::Position
				>();

		for (const auto [entity, total_working_time, working_time, emission_interval, emission_cooldown, particle, position]: view.each())
		{
			working_time.time += delta;

			// 仅在工作时间小于总工作时间时才发射粒子
			if (working_time.time < total_working_time.time)
			{
				emission_cooldown.cooldown -= delta;

				// 仅在冷却完毕时才发射粒子
				if (emission_cooldown.cooldown <= sf::Time::Zero)
				{
					factory::Particle::spawn(registry, particle.particle, entity);

					emission_cooldown.cooldown += emission_interval.interval;
				}
			}
		}
	}
}
