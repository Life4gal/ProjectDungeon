// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <assembly/transform.hpp>

#include <component/transform.hpp>

#include <entt/entt.hpp>

namespace pd::assembly
{
	using namespace component;

	auto Transform::make(
		entt::registry& registry,
		const entt::entity entity,
		const blueprint::Position position,
		const blueprint::Scale scale,
		const blueprint::Rotation rotation
	) noexcept -> void
	{
		registry.emplace<transform::Position>(entity, sf::Vector2f{position.x, position.y});
		registry.emplace<transform::Scale>(entity, sf::Vector2f{scale.x, scale.y});
		registry.emplace<transform::Rotation>(entity, sf::degrees(rotation.rotation));
	}
}
