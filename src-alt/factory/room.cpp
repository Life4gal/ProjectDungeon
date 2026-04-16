// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/room.hpp>

#include <blueprint/room.hpp>

#include <components/tags.hpp>
#include <components/room.hpp>

#include <factory/door.hpp>
#include <factory/chest.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace components;

	auto Room::spawn(entt::registry& registry, const type_type type, const position_type position) noexcept -> entt::entity
	{
		// todo
		const auto layout = blueprint::Room::generate_standard();

		const auto entity = registry.create();

		registry.emplace<room::Layout>(entity, layout);
		registry.emplace<room::Type>(entity, type);
		registry.emplace<room::Position>(entity, room::Position{position.x, position.y});

		registry.emplace<room::Doors>(entity);
		registry.emplace<room::Items>(entity);
		registry.emplace<room::Chests>(entity);
		registry.emplace<room::BloodStains>(entity);
		registry.emplace<room::Corpses>(entity);
		registry.emplace<room::Destroyable>(entity);

		return entity;
	}

	auto Room::destroy(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.destroy(entity);
	}

	auto Room::destroy_all(entt::registry& registry) noexcept -> void
	{
		Door::destroy_all(registry);
		Chest::destroy_all(registry);

		const auto view = registry.view<tags::room>();
		registry.destroy(view.begin(), view.end());
	}
}
