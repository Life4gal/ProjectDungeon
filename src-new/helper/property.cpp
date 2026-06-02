// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/Property.hpp>

#include <manager/event.hpp>

#include <event/actor.hpp>

#include <component/property.hpp>
#include <component/name.hpp>
#include <component/tags.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::helper
{
	using namespace component;

	auto Property::get_health(entt::registry& registry, const entt::entity entity) noexcept -> float
	{
		const auto* health = registry.try_get<const property::Health>(entity);
		if (health == nullptr)
		{
			return invalid_value;
		}

		return health->health;
	}

	auto Property::get_max_health(entt::registry& registry, const entt::entity entity) noexcept -> float
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

	auto Property::get_mana(entt::registry& registry, const entt::entity entity) noexcept -> float
	{
		const auto* mana = registry.try_get<const property::Mana>(entity);
		if (mana == nullptr)
		{
			return invalid_value;
		}

		return mana->mana;
	}

	auto Property::get_max_mana(entt::registry& registry, const entt::entity entity) noexcept -> float
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

	auto Property::hurt(entt::registry& registry, const entt::entity victim, const entt::entity attacker, const float damage) noexcept -> void
	{
		auto* health = registry.try_get<property::Health>(victim);
		auto* damage_history = registry.try_get<property::DamageHistory>(victim);

		if (health == nullptr or damage_history == nullptr)
		{
			return;
		}

		health->health -= damage;
		damage_history->damage_history.emplace_back(attacker, damage);

		const auto* victim_name = registry.try_get<name::Name>(victim);
		const auto* attacher_name = registry.try_get<name::Name>(attacker);

		SPDLOG_INFO(
			"{}(0x{:08X})受到来自{}(0x{:08X})的{:.1f}点伤害,剩余{:.1f}点生命值",
			victim_name ? victim_name->name : "实体",
			entt::to_integral(victim),
			attacher_name ? attacher_name->name : "实体",
			entt::to_integral(attacker),
			damage,
			health->health
		);

		manager::Event::enqueue(event::actor::Damaged{.victim = victim, .attacker = attacker, .damage = damage});
	}

	auto Property::kill([[maybe_unused]] entt::registry& registry, const entt::entity victim, const entt::entity attacker) noexcept -> void
	{
		// TODO: 需要做什么?
		registry.emplace_or_replace<state::EntityDead>(victim);

		const auto* victim_name = registry.try_get<name::Name>(victim);
		const auto* attacher_name = registry.try_get<name::Name>(attacker);

		SPDLOG_INFO(
			"{}(0x{:08X})被{}(0x{:08X})击杀",
			victim_name ? victim_name->name : "实体",
			entt::to_integral(victim),
			attacher_name ? attacher_name->name : "实体",
			entt::to_integral(attacker)
		);

		manager::Event::enqueue(event::actor::Slayed{.victim = victim, .attacker = attacker});
	}
}
