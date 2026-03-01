// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/entity.hpp>
#include <SFML/System/Vector2.hpp>
#include <box2d/math_functions.h>
#include <spdlog/spdlog.h>

namespace pd::log
{
	inline auto on_create(
		const std::string_view name,
		const entt::entity entity,
		const sf::Vector2f position,
		const sf::Vector2f size,
		const sf::Angle rotation,
		const b2Vec2 physics_position,
		const b2Vec2 physics_size
	) noexcept -> void
	{
		SPDLOG_INFO(
			"\n-->创建{}[{}]:\n"
			"[Pixel] Position: ({:.1f}.{:.1f}), Size: ({:.1f}.{:.1f}), Rotation: ({:.3f})\n"
			"[Physics] Position: ({:.1f}.{:.1f}), Size: ({:.1f}.{:.1f})",
			name,
			entt::to_integral(entity),
			position.x,
			position.y,
			size.x,
			size.y,
			rotation.asDegrees(),
			physics_position.x,
			physics_position.y,
			physics_size.x,
			physics_size.y
		);
	}

	inline auto on_create(
		const std::string_view name,
		const entt::entity entity,
		const sf::Vector2f position,
		const sf::Vector2f size,
		const sf::Angle rotation
	) noexcept -> void
	{
		SPDLOG_INFO(
			"\n-->创建{}[{}]:\n"
			"[Pixel] Position: ({:.1f}.{:.1f}), Size: ({:.1f}.{:.1f}), Rotation: ({:.3f})",
			name,
			entt::to_integral(entity),
			position.x,
			position.y,
			size.x,
			size.y,
			rotation.asDegrees()
		);
	}

	inline auto on_destroy(const std::string_view name, const entt::entity entity) noexcept -> void
	{
		SPDLOG_INFO("销毁{}[{}]", name, entt::to_integral(entity));
	}
}
