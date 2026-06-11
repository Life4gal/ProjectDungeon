// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <accessor/transform.hpp>

#include <component/transform.hpp>

#include <entt/entt.hpp>

namespace pd::accessor
{
	using namespace component;

	auto Transform::get_position(entt::registry& registry, const entt::entity entity) noexcept -> sf::Vector2f
	{
		const auto* position = registry.try_get<const transform::Position>(entity);
		if (position == nullptr)
		{
			return {0, 0};
		}

		return position->position;
	}

	auto Transform::set_position(entt::registry& registry, const entt::entity entity, const sf::Vector2f new_position) noexcept -> void
	{
		auto* position = registry.try_get<transform::Position>(entity);

		if (position == nullptr)
		{
			return;
		}

		position->position = new_position;
	}

	auto Transform::translate(entt::registry& registry, const entt::entity entity, const sf::Vector2f distance) noexcept -> void
	{
		auto* position = registry.try_get<transform::Position>(entity);

		if (position == nullptr)
		{
			return;
		}

		position->position += distance;
	}

	auto Transform::get_rotation(entt::registry& registry, const entt::entity entity) noexcept -> sf::Angle
	{
		const auto* rotation = registry.try_get<const transform::Rotation>(entity);
		if (rotation == nullptr)
		{
			return sf::degrees(0);
		}

		return rotation->rotation;
	}

	auto Transform::set_rotation(entt::registry& registry, const entt::entity entity, const sf::Angle new_rotation) noexcept -> void
	{
		auto* rotation = registry.try_get<transform::Rotation>(entity);
		if (rotation == nullptr)
		{
			return;
		}

		rotation->rotation = new_rotation;
	}
}
