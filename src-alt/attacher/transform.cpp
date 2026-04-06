// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <attacher/transform.hpp>

#include <components/transform.hpp>

// #include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>

namespace pd::attacher
{
	using namespace components;

	auto Transform::attach(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f position,
		const sf::Vector2f scale,
		const sf::Angle rotation
	) noexcept -> void
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
		// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<transform::Position>(entity));
		// 	return registry.get<const transform::Position>(entity).position;

		const auto* position = registry.try_get<const transform::Position>(entity);
		if (position == nullptr)
		{
			return {0, 0};
		}

		return position->position;
	}

	// auto Transform::set_position(entt::registry& registry, const entt::entity entity, const sf::Vector2f new_position) noexcept -> void
	// {
	// 	// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<transform::Position>(entity));
	// 	// 	registry.replace<transform::Position>(entity, position);
	//
	// 	auto* position = registry.try_get<transform::Position>(entity);
	// 	if (position == nullptr)
	// 	{
	// 		return;
	// 	}
	//
	// 	position->position = new_position;
	// }

	auto Transform::get_scale(entt::registry& registry, const entt::entity entity) noexcept -> sf::Vector2f
	{
		// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<transform::Scale>(entity));
		// 	return registry.get<const transform::Scale>(entity).scale;

		const auto* scale = registry.try_get<const transform::Scale>(entity);
		if (scale == nullptr)
		{
			return {1, 1};
		}

		return scale->scale;
	}

	auto Transform::set_scale(entt::registry& registry, const entt::entity entity, const sf::Vector2f new_scale) noexcept -> void
	{
		// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<transform::Scale>(entity));
		// 	registry.replace<transform::Scale>(entity, new_scale);

		auto* scale = registry.try_get<transform::Scale>(entity);
		if (scale == nullptr)
		{
			return;
		}

		scale->scale = new_scale;
	}

	auto Transform::get_rotation(entt::registry& registry, const entt::entity entity) noexcept -> sf::Angle
	{
		// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<transform::Rotation>(entity));
		// 	return registry.get<const transform::Rotation>(entity).rotation;

		const auto* rotation = registry.try_get<const transform::Rotation>(entity);
		if (rotation == nullptr)
		{
			return sf::degrees(0);
		}

		return rotation->rotation;
	}

	// auto Transform::set_rotation(entt::registry& registry, const entt::entity entity, const sf::Angle new_rotation) noexcept -> void
	// {
	// 	// 	PROMETHEUS_PLATFORM_ASSUME(registry.all_of<transform::Rotation>(entity));
	// 	// 	registry.replace<transform::Rotation>(entity, new_rotation);
	//
	// 	auto* rotation = registry.try_get<transform::Rotation>(entity);
	// 	if (rotation == nullptr)
	// 	{
	// 		return;
	// 	}
	//
	// 	rotation->rotation = new_rotation;
	// }
}
