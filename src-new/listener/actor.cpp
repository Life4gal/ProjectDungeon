// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/actor.hpp>

#include <component/actor.hpp>
#include <component/state.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::listener::actor
{
	auto on_hurt(entt::registry& registry, const event::actor::Hurt& hurt) noexcept -> void
	{
		namespace actor = component::actor;

		auto& [health] = registry.get<actor::Health>(hurt.victim);
		auto& [damage_history] = registry.get<actor::DamageHistory>(hurt.victim);

		health -= hurt.damage;
		damage_history.emplace_back(hurt.attacker, hurt.damage);

		SPDLOG_INFO("实体(0x{:08X})受到来自实体(0x{:08X})的{:.1f}点伤害,剩余{:.1f}点生命值", entt::to_integral(hurt.victim), entt::to_integral(hurt.attacker), hurt.damage, health);

		// 死亡计算不在这里进行
		// 见update::actor

		// TODO: 受击效果?
	}

	auto on_dead(entt::registry& registry, const event::actor::Dead& dead) noexcept -> void
	{
		namespace state = component::state;

		SPDLOG_INFO("实体(0x{:08X})被实体(0x{:08X})击杀", entt::to_integral(dead.victim), entt::to_integral(dead.attacker));

		registry.emplace<state::Dead>(dead.victim);
	}
}
