// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <accessor/property.hpp>

#include <component/property.hpp>

#include <entt/entt.hpp>

namespace pd::accessor
{
	using namespace component;

	auto Property::get_health(entt::registry& registry, const entt::entity entity) noexcept -> value_type
	{
		const auto* health = registry.try_get<const property::Health>(entity);
		if (health == nullptr)
		{
			return invalid_value;
		}

		return health->health;
	}

	auto Property::get_max_health(entt::registry& registry, const entt::entity entity) noexcept -> value_type
	{
		const auto* max_health = registry.try_get<const property::HealthMax>(entity);
		if (max_health == nullptr)
		{
			return invalid_value;
		}

		return max_health->health;
	}

	auto Property::set_health(entt::registry& registry, const entt::entity entity, const float new_health) noexcept -> void
	{
		auto* health = registry.try_get<property::Health>(entity);
		if (health == nullptr)
		{
			return;
		}

		health->health = new_health;
	}

	auto Property::set_max_health(entt::registry& registry, const entt::entity entity, const float new_max_health) noexcept -> void
	{
		auto* max_health = registry.try_get<property::HealthMax>(entity);
		if (max_health == nullptr)
		{
			return;
		}

		max_health->health = new_max_health;
	}

	auto Property::get_mana(entt::registry& registry, const entt::entity entity) noexcept -> value_type
	{
		const auto* mana = registry.try_get<const property::Mana>(entity);
		if (mana == nullptr)
		{
			return invalid_value;
		}

		return mana->mana;
	}

	auto Property::get_max_mana(entt::registry& registry, const entt::entity entity) noexcept -> value_type
	{
		const auto* max_mana = registry.try_get<const property::ManaMax>(entity);
		if (max_mana == nullptr)
		{
			return invalid_value;
		}

		return max_mana->mana;
	}

	auto Property::set_mana(entt::registry& registry, const entt::entity entity, const float new_mana) noexcept -> void
	{
		auto* mana = registry.try_get<property::Mana>(entity);
		if (mana == nullptr)
		{
			return;
		}

		mana->mana = new_mana;
	}

	auto Property::set_max_mana(entt::registry& registry, const entt::entity entity, const float new_max_mana) noexcept -> void
	{
		auto* max_mana = registry.try_get<property::ManaMax>(entity);
		if (max_mana == nullptr)
		{
			return;
		}

		max_mana->mana = new_max_mana;
	}
}
