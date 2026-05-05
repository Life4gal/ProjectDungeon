// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/projectile.hpp>

#include <entt/entity/fwd.hpp>

#include <SFML/System/Vector2.hpp>

namespace pd::factory
{
	class Projectile final
	{
	public:
		static auto spawn(entt::registry& registry, const blueprint::Projectile& projectile, entt::entity owner, sf::Vector2f direction) noexcept -> entt::entity;

		static auto destroy_all(entt::registry& registry) noexcept -> void;
	};
}
