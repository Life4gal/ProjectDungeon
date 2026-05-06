// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/sprite_effect.hpp>

#include <entt/fwd.hpp>

namespace pd::helper
{
	class SpriteEffect final
	{
	public:
		class Scale final
		{
		public:
			static auto linear(entt::registry& registry, entt::entity entity, sf::Vector2f from, sf::Vector2f to, sf::Time duration) noexcept -> void;

			static auto shockwave(entt::registry& registry, entt::entity entity, sf::Vector2f base, float peak, sf::Time duration) noexcept -> void;

			static auto bounce(entt::registry& registry, entt::entity entity, sf::Vector2f from, sf::Vector2f to, float stiffness, sf::Time duration) noexcept -> void;

			static auto breathing(entt::registry& registry, entt::entity entity, sf::Vector2f base, float min, float max, float speed, sf::Time duration) noexcept -> void;
		};

		class Color final
		{
		public:
			static auto linear(entt::registry& registry, entt::entity entity, sf::Color from, sf::Color to, sf::Time duration) noexcept -> void;
		};
	};
}
