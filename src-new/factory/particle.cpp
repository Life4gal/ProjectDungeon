// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/particle.hpp>

#include <component/particle.hpp>
#include <component/name.hpp>

#include <assembly/transform.hpp>
#include <assembly/render.hpp>
#include <assembly/render_effect.hpp>

#include <factory/scheduled_task.hpp>

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>

namespace pd::factory
{
	using namespace component;

	auto Particle::spawn(entt::registry& registry, const blueprint::Particle& particle, const entt::entity owner) noexcept -> entt::entity
	{
		const auto [owner_position] = registry.get<const transform::Position>(owner);
		const auto position = blueprint::Position{.x = owner_position.x, .y = owner_position.y};

		const auto entity = registry.create();

		const auto* owner_name = registry.try_get<name::Name>(owner);
		SPDLOG_INFO(
			"{}(0x{:08X})发射粒子(Entity: 0x{:08X}, Position: ({:.1f}:{:.1f}))",
			owner_name ? owner_name->name : "实体",
			entt::to_integral(owner),
			entt::to_integral(entity),
			position.x,
			position.y
		);

		// transform
		assembly::Transform::make(registry, entity, position);
		// scheduled_task
		ScheduledTask::spawn(registry, entity, particle.lifetime);
		// render
		assembly::Render::make(registry, entity, particle.sprite, blueprint::RenderLayer::PARTICLE);
		// render effect
		assembly::RenderEffect::make(registry, entity);
		assembly::RenderEffect::Color::alpha_fade(
			registry,
			entity,
			particle.start_alpha,
			particle.end_alpha,
			sf::milliseconds(particle.lifetime.time_ms).asSeconds()
		);
		assembly::RenderEffect::Scale::shrink(
			registry,
			entity,
			{particle.start_scale, particle.start_scale},
			{particle.end_scale, particle.end_scale},
			sf::milliseconds(particle.lifetime.time_ms).asSeconds()
		);
		// owner
		registry.emplace<particle::Owner>(entity, owner);
		// tags
		registry.emplace<tags::Particle>(entity);

		// 新生成
		registry.emplace<state::EntityNew>(entity);

		return entity;
	}

	auto Particle::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::Particle>();
		registry.destroy(view.begin(), view.end());
	}
}
