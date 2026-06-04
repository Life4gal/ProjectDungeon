// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/particle_effect.hpp>

#include <SFML/Graphics/Color.hpp>

namespace pd::designer
{
	auto ParticleEffect::projectile_trail() noexcept -> blueprint::ParticleEmitterConfig
	{
		return blueprint::ParticleEmitterConfig
		{
				.layers =
				{
						// 背景层(Alpha混合,淡出+缩小)
						{
								.sprite = {.texture = "./assets/projectile_standard.png", .uv_position = {.x = 0, .y = 24}, .uv_size = {.width = 24, .height = 24}, .pivot = {.x = 12, .y = 12}},
								.blend_mode = blueprint::ParticleBlendMode::ALPHA,
								.shader_path = "./assets/shaders/projectile_trail",
								.start_color = sf::Color::White.toInteger(),
								.end_color = sf::Color{255, 255, 255, 0}.toInteger(),
								.start_scale = 1.0f,
								.end_scale = 0.5f,
						},
						// 发光层(Add混合,淡出+缩小)
						{
								.sprite = {.texture = "./assets/projectile_standard.png", .uv_position = {.x = 0, .y = 48}, .uv_size = {.width = 24, .height = 24}, .pivot = {.x = 12, .y = 12}},
								.blend_mode = blueprint::ParticleBlendMode::ADD,
								.shader_path = "./assets/shaders/projectile_trail",
								.start_color = sf::Color::White.toInteger(),
								.end_color = sf::Color{255, 255, 255, 0}.toInteger(),
								.start_scale = 1.0f,
								.end_scale = 0.5f,
						},
				},
				.working_time_ms = 2050,
				.emission_interval_ms = 200,
				.particle_lifetime_ms = 500,
				.particle_acceleration = {.x = 0.0f, .y = 0.0f},
		};
	}
}
