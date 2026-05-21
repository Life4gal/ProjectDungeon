// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/projectile.hpp>

#include <component/projectile.hpp>

#include <helper/actor.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::helper
{
	using namespace component;

	auto Projectile::contact(entt::registry& registry, const entt::entity projectile, const entt::entity other) noexcept -> void
	{
		SPDLOG_INFO(
			"飞弹实体(0x{:08X})命中实体(0x{:08X})!",
			entt::to_integral(projectile),
			entt::to_integral(other)
		);

		if (registry.any_of<tags::Player, tags::Enemy>(other))
		{
			const auto [owner] = registry.get<const projectile::Owner>(projectile);
			const auto [damage] = registry.get<const projectile::Damage>(projectile);

			Actor::hurt(registry, other, owner, damage);
		}

		// TODO: 何时销毁飞弹?
		// registry.destroy(projectile);
		// 这里应该使用emplace_or_replace,而不是emplace,因为飞弹可能同时接触到多个物理体,也就是一帧内处理多个接触事件
		// TODO: 但是这也意味着一发飞弹可以同时命中多个敌人,造成多次伤害,这是我们预期的行为吗?
		registry.emplace_or_replace<state::entity::Dead>(projectile);
	}
}
