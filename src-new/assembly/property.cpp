// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <assembly/property.hpp>

#include <component/property.hpp>
#include <component/damage_statistics.hpp>

#include <entt/entt.hpp>

namespace pd::assembly
{
	using namespace component;

	auto Property::make(entt::registry& registry, const entt::entity entity, const blueprint::Property& property) noexcept -> void
	{
		// =====================
		// PROPERTY
		// =====================

		// 生命值
		registry.emplace<property::Health>(entity, property.health);
		registry.emplace<property::HealthMax>(entity, property.health);
		// 魔法值
		registry.emplace<property::Mana>(entity, property.mana);
		registry.emplace<property::ManaMax>(entity, property.mana);

		// TODO: 其他字段如何使用?

		// =====================
		// DAMAGE STATISTICS
		// =====================

		registry.emplace<damage_statistics::AttackRecords>(entity);
		registry.emplace<damage_statistics::InjuryRecords>(entity);
	}
}
