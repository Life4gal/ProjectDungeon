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

	auto particle_emitter(entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		// 所有不处于冷却的发射器
		// particle_emitter::Cooldown由scheduled_task负责移除
		for (const auto view = registry.view<const particle_emitter::Particle, const transform::Position>(entt::exclude<particle_emitter::Cooldown>);
		     const auto [entity, particle, position]: view.each())
		{
			// 进入冷却
			registry.emplace<particle_emitter::Cooldown>(entity);
			// 生成粒子
			factory::Particle::spawn(registry, particle.particle, entity);
		}
	}
}
