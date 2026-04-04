// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/chest.hpp>

#include <manager/event.hpp>

#include <components/tags.hpp>

#include <entt/entt.hpp>

namespace pd::systems
{
	auto Chest::subscribe_events(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::subscribe<room::DisableChest, &Chest::on_disable>(registry);
		Event::subscribe<room::EnableChest, &Chest::on_enable>(registry);
		Event::subscribe<chest::Contacted, &Chest::on_contact_chest>(registry);
	}

	auto Chest::unsubscribe_events(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::unsubscribe<room::DisableChest, &Chest::on_disable>(registry);
		Event::unsubscribe<room::EnableChest, &Chest::on_enable>(registry);
		Event::unsubscribe<chest::Contacted, &Chest::on_contact_chest>(registry);
	}

	auto Chest::on_disable(entt::registry& registry, const events::room::DisableChest& event) noexcept -> void
	{
		using namespace components;

		for (const auto entity: event.entities)
		{
			registry.emplace_or_replace<tags::disabled>(entity);
		}
	}

	auto Chest::on_enable(entt::registry& registry, const events::room::EnableChest& event) noexcept -> void
	{
		using namespace components;

		for (const auto entity: event.entities)
		{
			registry.remove<tags::disabled>(entity);
		}
	}

	auto Chest::on_contact_chest(entt::registry& registry, const events::chest::Contacted& contacted) noexcept -> void
	{
		//
	}

	auto Chest::spawn(entt::registry& registry) noexcept -> entt::entity
	{
		using namespace components;

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
		using namespace components;

		const auto view = registry.view<tags::chest>();
		registry.destroy(view.begin(), view.end());
	}
}
