// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <box2d/id.h>
#include <box2d/math_functions.h>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

namespace pd::systems
{
	class PhysicsWorld final
	{
	public:
		static auto create(entt::registry& registry) noexcept -> void;

		static auto destroy(entt::registry& registry) noexcept -> void;

		[[nodiscard]] static auto id(entt::registry& registry) noexcept -> b2WorldId;

		[[nodiscard]] static auto to_user_data(entt::entity entity) noexcept -> void*;

		[[nodiscard]] static auto to_entity(const void* user_data) noexcept -> entt::entity;

		[[nodiscard]] static auto physics_position_of(sf::Vector2f position) noexcept -> b2Vec2;

		[[nodiscard]] static auto position_of(b2Vec2 physics_position) noexcept -> sf::Vector2f;

		[[nodiscard]] static auto physics_size_of(sf::Vector2f size) noexcept -> b2Vec2;

		[[nodiscard]] static auto size_of(b2Vec2 physics_size) noexcept -> sf::Vector2f;

		[[nodiscard]] static auto physics_rotation_of(sf::Angle angle) noexcept -> b2Rot;

		[[nodiscard]] static auto rotation_of(b2Rot physics_angle) noexcept -> sf::Angle;
	};
}
