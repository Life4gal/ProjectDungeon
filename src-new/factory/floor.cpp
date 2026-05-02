// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/floor.hpp>

#include <component/floor.hpp>

#include <factory/detail/transform.hpp>
#include <factory/detail/sprite.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	auto Floor::spawn(entt::registry& registry, const blueprint::Floor& floor) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// transform
		detail::attach(registry, entity, floor.transform);
		// sprite
		detail::attach(registry, entity, floor.sprite);

		registry.emplace<floor::Floor>(entity);

		return entity;
	}

	auto Floor::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<floor::Floor>();
		registry.destroy(view.begin(), view.end());
	}
}
