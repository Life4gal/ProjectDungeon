// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/particle_effect.hpp>

#include <manager/resource.hpp>

#include <component/particle_effect.hpp>

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>

namespace pd::factory::detail
{
	using namespace component;

	namespace
	{
		auto to_sf_blend_mode(const blueprint::ParticleBlendMode mode) noexcept -> sf::BlendMode
		{
			switch (mode)
			{
				case blueprint::ParticleBlendMode::ALPHA:
				{
					return sf::BlendAlpha;
				}
				case blueprint::ParticleBlendMode::ADD:
				{
					return sf::BlendAdd;
				}
				case blueprint::ParticleBlendMode::MULTIPLY:
				{
					return sf::BlendMultiply;
				}
			}

			return sf::BlendAlpha;
		}
	}

	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::ParticleEmitterConfig& config) noexcept -> void
	{
		// render layers
		{
			auto& [layers] = registry.emplace<particle_effect::RenderLayers>(entity);
			layers.reserve(config.layers.size());

			for (const auto& [sprite, blend_mode, shader_path, start_color, end_color, start_scale, end_scale]: config.layers)
			{
				auto texture = manager::Texture::load(std::filesystem::path{sprite.texture});
				auto shader = manager::Shader::load(std::filesystem::path{shader_path});

				layers.push_back(
					{
							.texture = std::move(texture),
							.uv_position = {sprite.uv_position.x, sprite.uv_position.y},
							.uv_size = {sprite.uv_size.width, sprite.uv_size.height},
							.pivot = {sprite.pivot.x, sprite.pivot.y},
							.blend_mode = to_sf_blend_mode(blend_mode),
							.shader = std::move(shader),
							.start_color = sf::Color{start_color},
							.end_color = sf::Color{end_color},
							.start_scale = start_scale,
							.end_scale = end_scale,
					}
				);
			}
		}
		// working time
		registry.emplace<particle_effect::TotalWorkingTime>(entity, sf::milliseconds(config.working_time_ms));
		registry.emplace<particle_effect::WorkingTime>(entity, sf::Time::Zero);
		// emission interval&cooldown
		registry.emplace<particle_effect::EmissionInterval>(entity, sf::milliseconds(config.emission_interval_ms));
		registry.emplace<particle_effect::EmissionCooldown>(entity, sf::Time::Zero);
		// particle lifetime&acceleration
		registry.emplace<particle_effect::ParticleLifetime>(entity, sf::milliseconds(config.particle_lifetime_ms));
		registry.emplace<particle_effect::ParticleAcceleration>(entity, sf::Vector2f{config.particle_acceleration.x, config.particle_acceleration.y});
		// particles
		{
			const auto max_particle = config.working_time_ms / config.emission_interval_ms;

			auto& [particles] = registry.emplace<particle_effect::Particles>(entity);
			particles.reserve(max_particle);
		}
	}
}
