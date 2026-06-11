// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <assembly/detail/name.hpp>

#include <manager/i18n.hpp>

#include <component/name.hpp>

#include <entt/entt.hpp>

namespace pd::assembly
{
	using namespace component;

	// 必须和blueprint::NameType的枚举值顺序一致
	constexpr std::array NameI18NKeys
	{
			// 玩家
			"NAME.PLAYER.DEFAULT",

			// 敌人
			"NAME.ENEMY.RAT",
			"NAME.ENEMY.SLIME",
			"NAME.ENEMY.BAT",

			// NPC
			"NAME.NPC.MERCHANT",

			// 物品
			"NAME.ITEM.HEALTH_POTION",
	};

	auto Name::make(entt::registry& registry, const entt::entity entity, const blueprint::Name& name) noexcept -> void
	{
		const auto t_name = manager::Internationalization::map(NameI18NKeys[std::to_underlying(name.type)]);

		registry.emplace<name::NameType>(entity, name.type);
		registry.emplace<name::Name>(entity, t_name);
	}
}
