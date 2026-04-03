// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/tile.hpp>

#include <manager/event.hpp>

#include <components/tags.hpp>

#include <entt/entt.hpp>

namespace pd::systems
{
	auto Tile::subscribe_events(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		// using namespace events;
	}

	auto Tile::unsubscribe_events(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		// using namespace events;
	}

	auto Tile::create(entt::registry& registry) noexcept -> entt::entity
	{
		using namespace components;

		const auto entity = registry.create();

		// todo


		registry.emplace<tags::tile>(entity);
		return entity;
	}

	auto Tile::destroy(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.destroy(entity);
	}

	auto Tile::destroy_all(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		using namespace components;

		const auto view = registry.view<tags::tile>();
		registry.destroy(view.begin(), view.end());
	}

	auto Tile::is_walkable(const collision_logical collision_logical) noexcept -> bool
	{
		return collision_logical == collision_logical::FLOOR;
	}

	auto Tile::is_flyable(const collision_logical collision_logical) noexcept -> bool
	{
		return collision_logical != collision_logical::WALL;
	}

	auto Tile::is_passable(const collision_logical collision_logical) noexcept -> bool
	{
		return collision_logical != collision_logical::WALL and collision_logical != collision_logical::OBSTACLE;
	}
}
