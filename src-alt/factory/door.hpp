// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <components/door.hpp>

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>

namespace pd::listener
{
	class Door;
}

namespace pd::factory
{
	class Door final
	{
		// listener::Door需要重建门物理刚体
		friend listener::Door;

	public:
		using direction_type = components::door::Direction;
		using type_type = components::door::Type;

	private:
		static auto create_physics_body(entt::registry& registry, entt::entity entity, sf::Vector2f position, direction_type direction) noexcept -> void;

	public:
		// 创建门
		static auto spawn(entt::registry& registry, sf::Vector2f position, direction_type direction, type_type type) noexcept -> entt::entity;

		// 销毁门
		static auto destroy(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 销毁所有门
		static auto destroy_all(entt::registry& registry) noexcept -> void;
	};
}
