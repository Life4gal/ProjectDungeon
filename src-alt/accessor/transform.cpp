// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <accessor/transform.hpp>

#include <components/transform.hpp>

#include <entt/entt.hpp>

namespace pd::accessor
{
	using namespace components;


	auto Transform::initialize_system([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		//
	}

	auto Transform::destroy_system([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		//
	}

	auto Transform::attach(entt::registry& registry, const entt::entity entity, const sf::Vector2f position, const sf::Vector2f scale, const sf::Angle rotation) noexcept -> void
	{
		if (registry.any_of<transform::Position, transform::Scale, transform::Rotation>(entity))
		{
			return;
		}

		registry.emplace<transform::Position>(entity, position);
		registry.emplace<transform::Scale>(entity, scale);
		registry.emplace<transform::Rotation>(entity, rotation);
	}

	auto Transform::attach_force(entt::registry& registry, const entt::entity entity, const sf::Vector2f position, const sf::Vector2f scale, const sf::Angle rotation) noexcept -> void
	{
		registry.emplace_or_replace<transform::Position>(entity, position);
		registry.emplace_or_replace<transform::Scale>(entity, scale);
		registry.emplace_or_replace<transform::Rotation>(entity, rotation);
	}

	auto Transform::deattach(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.remove<transform::Position>(entity);
		registry.remove<transform::Scale>(entity);
		registry.remove<transform::Rotation>(entity);
	}

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

	auto Transform::get_scale(entt::registry& registry, const entt::entity entity) noexcept -> sf::Vector2f
	{
		const auto* scale = registry.try_get<const transform::Scale>(entity);
		if (scale == nullptr)
		{
			return {1, 1};
		}

		return scale->scale;
	}

	auto Transform::set_scale(entt::registry& registry, const entt::entity entity, const sf::Vector2f new_scale) noexcept -> void
	{
		auto* scale = registry.try_get<transform::Scale>(entity);
		if (scale == nullptr)
		{
			return;
		}

		scale->scale = new_scale;
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
