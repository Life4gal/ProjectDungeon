// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/chest.hpp>

#include <components/tags.hpp>
#include <components/room.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace components;

	auto Chest::spawn(entt::registry& registry) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// todo


		registry.emplace<tags::chest>(entity);
		return entity;
	}

	auto Chest::destroy(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.destroy(entity);
	}

	auto Chest::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::chest>();
		registry.destroy(view.begin(), view.end());
	}
}
