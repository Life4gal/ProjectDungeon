// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <trigger/property.hpp>

#include <manager/event.hpp>
#include <manager/clock.hpp>

#include <event/property.hpp>

#include <component/property.hpp>
#include <component/damage_statistics.hpp>
#include <component/transform.hpp>
#include <component/name.hpp>
#include <component/tags.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::trigger
{
	using namespace component;

	auto Property::hurt(entt::registry& registry, const entt::entity victim, const entt::entity attacker, const float damage) noexcept -> void
	{
		auto* health = registry.try_get<property::Health>(victim);
		if (health == nullptr)
		{
			return;
		}

		// 添加统计信息
		if (auto* attack_records = registry.try_get<damage_statistics::AttackRecords>(attacker);
			attack_records != nullptr)
		{
			const auto [victim_position] = registry.get<const transform::Position>(victim);
			const auto [attacker_position] = registry.get<const transform::Position>(attacker);
			const auto attack_index = attack_records->records.size();

			attack_records->records.push_back(
				{
						.victim = victim,
						.time = manager::Clock::now(),
						.attacker_position = attacker_position,
						.victim_position = victim_position,
						.victim_health = health->health,
						.damage = damage,
				}
			);

			if (auto* injury_records = registry.try_get<damage_statistics::InjuryRecords>(victim);
				injury_records != nullptr)
			{
				injury_records->records.push_back(
					{
							.attacker = attacker,
							.attack_index = attack_index
					}
				);
			}
		}

		health->health -= damage;

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

		manager::Event::enqueue(event::property::Damaged{.victim = victim, .attacker = attacker, .damage = damage});
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

		manager::Event::enqueue(event::property::Slayed{.victim = victim, .attacker = attacker});
	}
}
