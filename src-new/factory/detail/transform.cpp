// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/transform.hpp>

#include <component/transform.hpp>

#include <entt/entity/registry.hpp>

namespace pd::factory::detail
{
	using namespace component;

	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::Transform& transform) noexcept -> void
	{
		registry.emplace<transform::Position>(entity, sf::Vector2f{transform.x, transform.y});
		registry.emplace<transform::Scale>(entity, sf::Vector2f{transform.scale_x, transform.scale_y});
		registry.emplace<transform::Rotation>(entity, sf::degrees(transform.rotation));
	}
}
