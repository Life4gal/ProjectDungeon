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
		using namespace events;

		Event::subscribe<room::DisableTile, &on_disable>(registry);
		Event::subscribe<room::EnableTile, &on_enable>(registry);
	}

	auto Tile::unsubscribe_events(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::unsubscribe<room::DisableTile, &on_disable>(registry);
		Event::unsubscribe<room::EnableTile, &on_enable>(registry);
	}

	auto Tile::on_disable(entt::registry& registry, const events::room::DisableTile& event) noexcept -> void
	{
		using namespace components;

		for (const auto entity: event.entities)
		{
			registry.emplace_or_replace<tags::disabled>(entity);
		}
	}

	auto Tile::on_enable(entt::registry& registry, const events::room::EnableTile& event) noexcept -> void
	{
		using namespace components;

		for (const auto entity: event.entities)
		{
			registry.remove<tags::disabled>(entity);
		}
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
