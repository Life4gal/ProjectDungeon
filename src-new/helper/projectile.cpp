// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/projectile.hpp>

#include <component/projectile.hpp>
#include <component/name.hpp>

#include <helper/property.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::helper
{
	using namespace component;

	auto Projectile::contact(entt::registry& registry, const entt::entity projectile, const entt::entity other) noexcept -> void
	{
		const auto [owner] = registry.get<const projectile::Owner>(projectile);

		const auto* owner_name = registry.try_get<name::Name>(owner);
		const auto* other_name = registry.try_get<name::Name>(other);

		SPDLOG_INFO(
			"{}(0x{:08X})的飞弹实体(0x{:08X})命中{}(0x{:08X})!",
			owner_name ? owner_name->name : "实体",
			entt::to_integral(owner),
			entt::to_integral(projectile),
			other_name ? other_name->name : "实体",
			entt::to_integral(other)
		);

		if (registry.any_of<tags::Player, tags::Enemy>(other))
		{
			const auto [damage] = registry.get<const projectile::Damage>(projectile);

			Property::hurt(registry, other, owner, damage);
		}

		// TODO: 何时销毁飞弹?
		// registry.destroy(projectile);
		// 这里应该使用emplace_or_replace,而不是emplace,因为飞弹可能同时接触到多个物理体,也就是一帧内处理多个接触事件
		// TODO: 但是这也意味着一发飞弹可以同时命中多个敌人,造成多次伤害,这是我们预期的行为吗?
		registry.emplace_or_replace<state::EntityDead>(projectile);
	}
}
