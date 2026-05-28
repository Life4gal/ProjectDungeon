// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/enemy.hpp>

#include <component/enemy.hpp>

#include <helper/property.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::helper
{
	using namespace component;

	auto Enemy::contact(entt::registry& registry, const entt::entity enemy, const entt::entity other) noexcept -> void
	{
		// 接触的不是玩家则跳过
		// 目前还没有考虑接触其他类型的情况
		if (not registry.all_of<tags::Player>(other))
		{
			return;
		}

		SPDLOG_INFO(
			"敌人实体(0x{:08X})与玩家实体(0x{:08X})接触!",
			entt::to_integral(enemy),
			entt::to_integral(other)
		);

		const auto [contact_damage] = registry.get<const enemy::ContactDamage>(enemy);

		Property::hurt(registry, other, enemy, contact_damage);

		// TODO: 玩家受击动画?
	}
}
