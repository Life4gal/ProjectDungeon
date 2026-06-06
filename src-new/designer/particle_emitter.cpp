// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/particle_emitter.hpp>

#include <SFML/Graphics/Color.hpp>

namespace pd::designer
{
	auto ParticleEmitter::projectile_trail() noexcept -> blueprint::ParticleEmitter
	{
		blueprint::Particle particle
		{
				.sprite = {.texture = "./assets/projectile_standard.png", .uv_position = {.x = 0, .y = 24}, .uv_size = {.width = 24, .height = 24}, .pivot = {.x = 12, .y = 12}},
				.shader = "./assets/shaders/projectile_trail",
				.lifetime = {.lifetime_ms = 500},
				.start_color = sf::Color::White.toInteger(),
				.end_color = sf::Color{255, 255, 255, 0}.toInteger(),
				.start_scale = 0.8f,
				.end_scale = 0.25f,
		};

		return
		{
				.working_time_ms = 2050,
				.emission_interval_ms = 200,
				.particle = std::move(particle),
		};
	}
}
