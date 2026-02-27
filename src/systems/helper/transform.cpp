// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/transform.hpp>

#include <components/transform.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>

namespace pd::systems::helper
{
	auto Transform::attach(
		entt::registry& registry,
		const entt::entity entity,
		const sf::Vector2f position,
		const sf::Vector2f scale,
		const sf::Angle rotation
	) noexcept -> void
	{
		using namespace components;

		registry.emplace_or_replace<transform::Position>(entity, position);
		registry.emplace_or_replace<transform::Scale>(entity, scale);
		registry.emplace_or_replace<transform::Rotation>(entity, rotation);
	}

	auto Transform::deattach(entt::registry& registry, const entt::entity entity_with_transform) noexcept -> void
	{
		using namespace components;

		registry.remove<transform::Position>(entity_with_transform);
		registry.remove<transform::Scale>(entity_with_transform);
		registry.remove<transform::Rotation>(entity_with_transform);
	}

	auto Transform::get_position(entt::registry& registry, const entt::entity entity_with_transform) noexcept -> sf::Vector2f
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<transform::Position>(entity_with_transform));

		return registry.get<const transform::Position>(entity_with_transform).position;
	}

	auto Transform::set_position(entt::registry& registry, const entt::entity entity_with_transform, const sf::Vector2f position) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<transform::Position>(entity_with_transform));

		registry.replace<transform::Position>(entity_with_transform, position);
	}

	auto Transform::get_scale(entt::registry& registry, const entt::entity entity_with_transform) noexcept -> sf::Vector2f
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<transform::Scale>(entity_with_transform));

		return registry.get<const transform::Scale>(entity_with_transform).scale;
	}

	auto Transform::set_scale(entt::registry& registry, const entt::entity entity_with_transform, const sf::Vector2f scale) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<transform::Scale>(entity_with_transform));

		registry.replace<transform::Scale>(entity_with_transform, scale);
	}

	auto Transform::get_rotation(entt::registry& registry, const entt::entity entity_with_transform) noexcept -> sf::Angle
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<transform::Rotation>(entity_with_transform));

		return registry.get<const transform::Rotation>(entity_with_transform).rotation;
	}

	auto Transform::set_rotation(entt::registry& registry, const entt::entity entity_with_transform, const sf::Angle rotation) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<transform::Rotation>(entity_with_transform));

		registry.replace<transform::Rotation>(entity_with_transform, rotation);
	}
}
