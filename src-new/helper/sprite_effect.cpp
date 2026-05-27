// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/sprite_effect.hpp>

#include <algorithm>
#include <ranges>

#include <entt/entt.hpp>

namespace pd::helper
{
	using namespace component::render_effect::sprite;

	auto SpriteEffect::Position::linear(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f start,
		const sf::Vector2f end,
		const float speed
	) noexcept -> void
	{
		const auto offset = end - start;
		const auto distance = offset.length();

		const position::Linear linear
		{
				.start = start,
				.end = end,
				.speed = speed,
				.offset = offset,
				.distance = distance,
				.traveled = 0,
		};

		registry.emplace_or_replace<position::Linear>(entity, linear);
	}

	auto SpriteEffect::Position::oscillator(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f p1,
		const sf::Vector2f p2,
		const float speed,
		const float duration
	) noexcept -> void
	{
		const auto offset = p2 - p1;
		const auto distance = offset.length();

		const position::Oscillator oscillator
		{
				.p1 = p1,
				.p2 = p2,
				.speed = speed,
				.duration = duration,
				.offset = offset,
				.distance = distance,
				.elapsed = 0,
		};

		registry.emplace_or_replace<position::Oscillator>(entity, oscillator);
	}

	auto SpriteEffect::Position::spring(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f start,
		const sf::Vector2f target,
		const float strength,
		const float damping,
		const float duration
	) noexcept -> void
	{
		const position::Spring spring
		{
				.start = start,
				.target = target,
				.strength = strength,
				.damping = damping,
				.duration = duration,
				.velocity = {0, 0},
				.position = start,
				.elapsed = 0,
		};

		registry.emplace_or_replace<position::Spring>(entity, spring);
	}

	auto SpriteEffect::Position::path(
		entt::registry& registry,
		const entt::entity entity,
		std::vector<sf::Vector2f> waypoints,
		const float speed,
		const float duration
	) noexcept -> void
	{
		const auto segment_count = waypoints.size() - 1;
		const auto first_point = waypoints[0];

		std::vector<sf::Vector2f> offsets;
		std::vector<float> distances;

		offsets.reserve(segment_count);
		distances.reserve(segment_count);

		for (std::size_t i = 0; i < segment_count; ++i)
		{
			const auto& p0 = waypoints[i];
			const auto& p1 = waypoints[i + 1];

			const auto offset = p1 - p0;
			const auto distance = offset.length();

			offsets.emplace_back(offset);
			distances.emplace_back(distance);
		}

		position::Path path
		{
				.waypoints = std::move(waypoints),
				.speed = speed,
				.duration = duration,
				.offsets = std::move(offsets),
				.distances = std::move(distances),
				.current_segment = 0,
				.current_segment_traveled = 0,
				.position = first_point,
				.elapsed = 0,
		};

		registry.emplace_or_replace<position::Path>(entity, std::move(path));
	}

	auto SpriteEffect::Position::orbit(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f center,
		const float radius,
		const float speed,
		const float duration
	) noexcept -> void
	{
		const position::Orbit orbit
		{
				.center = center,
				.radius = radius,
				.speed = speed,
				.duration = duration,
				.angle = 0,
				.elapsed = 0,
		};

		registry.emplace_or_replace<position::Orbit>(entity, orbit);
	}

	auto SpriteEffect::Position::shake(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f center,
		const float intensity,
		const float frequency,
		const float decay,
		const float duration
	) noexcept -> void
	{
		const position::Shake shake
		{
				.center = center,
				.intensity = intensity,
				.frequency = frequency,
				.decay = decay,
				.duration = duration,
				.phase = 0,
				.elapsed = 0,
		};

		registry.emplace_or_replace<position::Shake>(entity, shake);
	}

	auto SpriteEffect::Position::wave(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f start,
		sf::Vector2f direction,
		const float amplitude,
		const float wavelength,
		const float speed,
		const float duration
	) noexcept -> void
	{
		direction = direction.normalized();

		const auto perpendicular_direction = direction.perpendicular();

		const position::Wave wave
		{
				.start = start,
				.direction = direction,
				.amplitude = amplitude,
				.wavelength = wavelength,
				.speed = speed,
				.duration = duration,
				.perpendicular_direction = perpendicular_direction,
				.traveled = 0,
				.elapsed = 0,
		};

		registry.emplace_or_replace<position::Wave>(entity, wave);
	}

	auto SpriteEffect::Position::swing(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f pivot,
		const float radius,
		const float start_angle,
		const float end_angle,
		const float speed,
		const float duration
	) noexcept -> void
	{
		const auto angle_diff = std::abs(end_angle - start_angle);
		const auto angle_total = angle_diff * 2;

		const position::Swing swing
		{
				.pivot = pivot,
				.radius = radius,
				.start_angle = start_angle,
				.end_angle = end_angle,
				.speed = speed,
				.duration = duration,
				.angle_diff = angle_diff,
				.angle_total = angle_total,
				.elapsed = 0,
		};

		registry.emplace_or_replace<position::Swing>(entity, swing);
	}

	auto SpriteEffect::Scale::oscillator(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f min,
		const sf::Vector2f max,
		const float speed,
		const float duration
	) noexcept -> void
	{
		const auto diff = max - min;
		const auto swing = diff * 2.0f;

		const scale::Oscillator oscillator
		{
				.min = min,
				.max = max,
				.speed = speed,
				.duration = duration,
				.diff = diff,
				.swing = swing,
				.elapsed = 0,
		};

		registry.emplace_or_replace<scale::Oscillator>(entity, oscillator);
	}

	auto SpriteEffect::Scale::spring(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f min,
		const sf::Vector2f max,
		const float strength,
		const float damping,
		const float duration
	) noexcept -> void
	{
		const scale::Spring spring
		{
				.min = min,
				.max = max,
				.strength = strength,
				.damping = damping,
				.duration = duration,
				.velocity = {0, 0},
				.scale = min,
				.elapsed = 0,
		};

		registry.emplace_or_replace<scale::Spring>(entity, spring);
	}

	auto SpriteEffect::Scale::breathing(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f min,
		const sf::Vector2f max,
		const float speed,
		const float duration
	) noexcept -> void
	{
		const auto diff = max - min;

		const scale::Breathing breathing
		{
				.min = min,
				.max = max,
				.speed = speed,
				.duration = duration,
				.diff = diff,
				.elapsed = 0,
		};

		registry.emplace_or_replace<scale::Breathing>(entity, breathing);
	}

	auto SpriteEffect::Scale::squash_stretch(
		entt::registry& registry,
		const entt::entity entity,
		const float squash_factory,
		const float speed,
		const float duration
	) noexcept -> void
	{
		const scale::SquashStretch squash_stretch
		{
				.squash_factory = squash_factory,
				.speed = speed,
				.duration = duration,
				.elapsed = 0,
		};

		registry.emplace_or_replace<scale::SquashStretch>(entity, squash_stretch);
	}

	auto SpriteEffect::Scale::directional_pulse(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f center,
		const sf::Vector2f direction,
		const sf::Vector2f scale,
		const float speed,
		const float duration
	) noexcept -> void
	{
		const scale::DirectionalPulse directional_pulse
		{
				.center = center,
				.direction = direction,
				.scale = scale,
				.speed = speed,
				.duration = duration,
				.elapsed = 0,
		};

		registry.emplace_or_replace<scale::DirectionalPulse>(entity, directional_pulse);
	}

	auto SpriteEffect::Scale::jelly(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f target_scale,
		const float overshoot,
		const float frequency,
		const float damping,
		const float duration
	) noexcept -> void
	{
		const scale::Jelly jelly
		{
				.target_scale = target_scale,
				.overshoot = overshoot,
				.frequency = frequency,
				.damping = damping,
				.duration = duration,
				.velocity = {0, 0},
				.current_scale = target_scale - target_scale * overshoot,
				.elapsed = 0,
		};

		registry.emplace_or_replace<scale::Jelly>(entity, jelly);
	}

	auto SpriteEffect::Scale::elastic_hit(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f compress_scale,
		const sf::Vector2f stretch_scale,
		const float speed,
		const float duration
	) noexcept -> void
	{
		const auto phase_duration = duration / 3;
		const auto compress_phase_diff = compress_scale - sf::Vector2f{1, 1};
		const auto stretch_phase_diff = stretch_scale - compress_scale;
		const auto resume_phase_diff = sf::Vector2f{1, 1} - stretch_scale;

		const scale::ElasticHit elastic_hit
		{
				.compress_scale = compress_scale,
				.stretch_scale = stretch_scale,
				.speed = speed,
				.duration = duration,
				.phase_duration = phase_duration,
				.compress_phase_diff = compress_phase_diff,
				.stretch_phase_diff = stretch_phase_diff,
				.resume_phase_diff = resume_phase_diff,
				.phase = scale::ElasticHit::Phase::COMPRESS,
				.phase_elapsed = 0,
				.elapsed = 0,
		};

		registry.emplace_or_replace<scale::ElasticHit>(entity, elastic_hit);
	}

	auto SpriteEffect::Scale::ripple(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f center_scale,
		const sf::Vector2f edge_scale,
		const float speed,
		const float duration
	) noexcept -> void
	{
		const auto scale_diff = edge_scale - center_scale;

		const scale::Ripple ripple
		{
				.center_scale = center_scale,
				.edge_scale = edge_scale,
				.speed = speed,
				.duration = duration,
				.scale_diff = scale_diff,
				.radius = 0,
				.elapsed = 0,
		};

		registry.emplace_or_replace<scale::Ripple>(entity, ripple);
	}

	auto SpriteEffect::Color::fade(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Color start,
		const sf::Color end,
		const float duration
	) noexcept -> void
	{
		const auto diff = end - start;

		const color::Fade fade
		{
				.start = start,
				.end = end,
				.duration = duration,
				.diff = diff,
				.elapsed = 0,
		};

		registry.emplace_or_replace<color::Fade>(entity, fade);
	}

	auto SpriteEffect::Color::alpha_fade(
		entt::registry& registry,
		const entt::entity entity,
		const float start,
		const float end,
		const float duration
	) noexcept -> void
	{
		const auto diff = end - start;

		const color::AlphaFade alpha_fade
		{
				.start = start,
				.end = end,
				.duration = duration,
				.diff = diff,
				.elapsed = 0,
		};

		registry.emplace_or_replace<color::AlphaFade>(entity, alpha_fade);
	}

	auto SpriteEffect::Color::flash(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Color base_color,
		const sf::Color flash_color,
		const float speed,
		const float decay,
		const float duration
	) noexcept -> void
	{
		const auto diff = flash_color - base_color;

		const color::Flash flash
		{
				.base_color = base_color,
				.flash_color = flash_color,
				.speed = speed,
				.decay = decay,
				.duration = duration,
				.diff = diff,
				.elapsed = 0,
		};

		registry.emplace_or_replace<color::Flash>(entity, flash);
	}

	auto SpriteEffect::Color::breathing(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Color min,
		const sf::Color max,
		const float speed,
		const float duration
	) noexcept -> void
	{
		const auto diff = max - min;

		const color::Breathing breathing
		{
				.min = min,
				.max = max,
				.speed = speed,
				.duration = duration,
				.diff = diff,
				.elapsed = 0,
		};

		registry.emplace_or_replace<color::Breathing>(entity, breathing);
	}

	auto SpriteEffect::Color::pulse(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Color base_color,
		const sf::Color peak_color,
		const float speed,
		const float peak_duration,
		const float duration
	) noexcept -> void
	{
		const auto diff = peak_color - base_color;
		const auto cycle_duration = 1.0f / speed;
		const auto raise_duration = (cycle_duration - peak_duration) / 2.0f;

		const color::Pulse pulse
		{
				.base_color = base_color,
				.peak_color = peak_color,
				.speed = speed,
				.peak_duration = peak_duration,
				.duration = duration,
				.diff = diff,
				.cycle_duration = cycle_duration,
				.raise_duration = raise_duration,
				.elapsed = 0,
		};

		registry.emplace_or_replace<color::Pulse>(entity, pulse);
	}

	auto SpriteEffect::Color::rainbow_cycle(
		entt::registry& registry,
		const entt::entity entity,
		const float speed,
		const float saturation,
		const float lightness,
		const float duration
	) noexcept -> void
	{
		const color::RainbowCycle rainbow_cycle
		{
				.speed = speed,
				.saturation = saturation,
				.lightness = lightness,
				.duration = duration,
				.elapsed = 0,
		};

		registry.emplace_or_replace<color::RainbowCycle>(entity, rainbow_cycle);
	}

	auto SpriteEffect::Color::oscillator(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Color color1,
		const sf::Color color2,
		const float speed,
		const float duration
	) noexcept -> void
	{
		const auto diff = color2 - color1;

		const color::Oscillator oscillator
		{
				.color1 = color1,
				.color2 = color2,
				.speed = speed,
				.duration = duration,
				.diff = diff,
				.elapsed = 0,
		};

		registry.emplace_or_replace<color::Oscillator>(entity, oscillator);
	}
}
