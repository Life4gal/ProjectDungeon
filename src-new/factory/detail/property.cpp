// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/property.hpp>

#include <component/property.hpp>
#include <component/property_state.hpp>

#include <entt/entt.hpp>

namespace pd::factory::detail
{
	using namespace component;

	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::Property& property) noexcept -> void
	{
		// 生命值
		registry.emplace<property::Health>(entity, property.health);
		registry.emplace<property::HealthMax>(entity, property.health);
		// 魔法值
		registry.emplace<property::Mana>(entity, property.mana);
		registry.emplace<property::ManaMax>(entity, property.mana);

		// TODO: 其他字段如何使用?
	}

	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::PropertyState& property_state) noexcept -> void
	{
		// 生命值条&魔法值条
		registry.emplace<property_state::HealthBarOffset>(entity, sf::Vector2f{property_state.health_bar_offset.x, property_state.health_bar_offset.y});
		registry.emplace<property_state::HealthBarSize>(entity, sf::Vector2f{property_state.health_bar_size.width, property_state.health_bar_size.height});
		registry.emplace<property_state::ManaBarOffset>(entity, sf::Vector2f{property_state.mana_bar_offset.x, property_state.mana_bar_offset.y});
		registry.emplace<property_state::ManaBarSize>(entity, sf::Vector2f{property_state.mana_bar_size.width, property_state.mana_bar_size.height});
		// 受伤记录
		registry.emplace<property_state::DamageHistory>(entity);
	}
}
