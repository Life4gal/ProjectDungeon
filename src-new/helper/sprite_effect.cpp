// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/sprite_effect.hpp>

#include <entt/entt.hpp>

namespace pd::helper
{
	using namespace component::sprite_effect;

	auto SpriteEffect::Scale::linear(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f from,
		const sf::Vector2f to,
		const sf::Time duration
	) noexcept -> void
	{
		registry.emplace_or_replace<scale::Linear>(entity, scale::Linear{.from = from, .to = to, .duration = duration, .elapsed = sf::Time::Zero});
	}

	auto SpriteEffect::Scale::shockwave(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f base,
		const float peak,
		const sf::Time duration
	) noexcept -> void
	{
		registry.emplace_or_replace<scale::Shockwave>(entity, scale::Shockwave{.base = base, .peak = peak, .duration = duration, .elapsed = sf::Time::Zero});
	}

	auto SpriteEffect::Scale::bounce(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f from,
		const sf::Vector2f to,
		const float stiffness,
		const sf::Time duration
	) noexcept -> void
	{
		registry.emplace_or_replace<scale::Bounce>(entity, scale::Bounce{.from = from, .to = to, .stiffness = stiffness, .duration = duration, .elapsed = sf::Time::Zero});
	}

	auto SpriteEffect::Scale::breathing(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f base,
		const float min,
		const float max,
		const float speed,
		const sf::Time duration
	) noexcept -> void
	{
		registry.emplace_or_replace<scale::Breathing>(entity, scale::Breathing{.base = base, .min = min, .max = max, .speed = speed, .duration = duration, .elapsed = sf::Time::Zero});
	}

	auto SpriteEffect::Color::linear(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Color from,
		const sf::Color to,
		const sf::Time duration
	) noexcept -> void
	{
		registry.emplace_or_replace<color::Linear>(entity, color::Linear{.from = from, .to = to, .duration = duration, .elapsed = sf::Time::Zero});
	}
}
