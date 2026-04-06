// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/chest.hpp>

#include <manager/event.hpp>

#include <components/tags.hpp>

#include <entt/entt.hpp>

namespace pd::listener
{
	auto Chest::subscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::subscribe<chest::Disable, &Chest::on_disable>(registry);
		Event::subscribe<chest::Enable, &Chest::on_enable>(registry);
		Event::subscribe<chest::DisableAll, &Chest::on_disable_all>(registry);
		Event::subscribe<chest::EnableAll, &Chest::on_enable_all>(registry);
		Event::subscribe<chest::Contacted, &Chest::on_contact>(registry);
	}

	auto Chest::unsubscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::unsubscribe<chest::Disable, &Chest::on_disable>(registry);
		Event::unsubscribe<chest::Enable, &Chest::on_enable>(registry);
		Event::unsubscribe<chest::DisableAll, &Chest::on_disable_all>(registry);
		Event::unsubscribe<chest::EnableAll, &Chest::on_enable_all>(registry);
		Event::unsubscribe<chest::Contacted, &Chest::on_contact>(registry);
	}

	auto Chest::on_disable(entt::registry& registry, const events::chest::Disable& event) noexcept -> void
	{
		using namespace components;

		registry.emplace_or_replace<tags::disabled>(event.chest);
	}

	auto Chest::on_enable(entt::registry& registry, const events::chest::Enable& event) noexcept -> void
	{
		using namespace components;

		registry.remove<tags::disabled>(event.chest);
	}

	auto Chest::on_disable_all(entt::registry& registry, const events::chest::DisableAll& event) noexcept -> void
	{
		using namespace components;

		for (const auto chest: event.chests)
		{
			registry.emplace_or_replace<tags::disabled>(chest);
		}
	}

	auto Chest::on_enable_all(entt::registry& registry, const events::chest::EnableAll& event) noexcept -> void
	{
		using namespace components;

		for (const auto chest: event.chests)
		{
			registry.remove<tags::disabled>(chest);
		}
	}

	auto Chest::on_contact(entt::registry& registry, const events::chest::Contacted& event) noexcept -> void
	{
		// todo
	}
}
