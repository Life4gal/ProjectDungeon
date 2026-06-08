// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <assembly/particle_emitter.hpp>

#include <component/particle_emitter.hpp>

#include <factory/scheduled_task.hpp>

#include <entt/entt.hpp>

namespace pd::assembly
{
	using namespace component;

	auto ParticleEmitter::make(entt::registry& registry, const entt::entity entity, const blueprint::ParticleEmitter& particle_emitter) noexcept -> void
	{
		// 定时 -> 移除发射器组件
		factory::ScheduledTask::spawn</*particle_emitter::Cooldown, */particle_emitter::Particle>(registry, entity, particle_emitter.emitter);
		// 定时 -> 发射粒子
		factory::ScheduledTask::spawn<particle_emitter::Cooldown>(registry, entity, particle_emitter.emission);
		// particle
		registry.emplace<particle_emitter::Particle>(entity, particle_emitter.particle);
	}
}
