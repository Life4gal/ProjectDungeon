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

		Event::subscribe<room::PreDisableChest, &Chest::on_pre_disable>(registry);
		Event::subscribe<room::PostDisableChest, &Chest::on_post_disable>(registry);
		Event::subscribe<room::PreEnableChest, &Chest::on_pre_enable>(registry);
		Event::subscribe<room::PostEnableChest, &Chest::on_post_enable>(registry);

		Event::subscribe<chest::Contacted, &Chest::on_contact_chest>(registry);
	}

	auto Chest::unsubscribe_events(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::unsubscribe<room::PreDisableChest, &Chest::on_pre_disable>(registry);
		Event::unsubscribe<room::PostDisableChest, &Chest::on_post_disable>(registry);
		Event::unsubscribe<room::PreEnableChest, &Chest::on_pre_enable>(registry);
		Event::unsubscribe<room::PostEnableChest, &Chest::on_post_enable>(registry);

		Event::unsubscribe<chest::Contacted, &Chest::on_contact_chest>(registry);
	}

	auto Chest::on_pre_disable(entt::registry& registry, const events::room::PreDisableChest& event) noexcept -> void
	{
		//
	}

	auto Chest::on_post_disable(entt::registry& registry, const events::room::PostDisableChest& event) noexcept -> void
	{
		//
	}

	auto Chest::on_pre_enable(entt::registry& registry, const events::room::PreEnableChest& event) noexcept -> void
	{
		//
	}

	auto Chest::on_post_enable(entt::registry& registry, const events::room::PostEnableChest& event) noexcept -> void
	{
		//
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
