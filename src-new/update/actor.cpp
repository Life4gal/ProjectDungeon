// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/actor.hpp>

#include <component/actor.hpp>
#include <component/player.hpp>

#include <helper/actor.hpp>
#include <helper/room.hpp>

#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;

	auto actor(entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		const auto view = registry
				.view<
					state::actor::Awake,
					actor::Health
				>();

		for (const auto [entity, health]: view.each())
		{
			if (health.health <= 0)
			{
				// 我们还没有处理玩家死亡逻辑 :)
				if (registry.all_of<tags::Player>(entity))
				{
					health.health = 100;
					continue;
				}

				// registry.emplace<state::actor::Dying>(entity);
				// 标记为可被销毁
				registry.emplace<state::entity::Dead>(entity);

				// TODO: 受伤记录应该如何利用?理论上这可以用于数据统计,但是如果我们将这部分数据保存在实体上,在实体被销毁后数据将不复存在
				const auto& [damage_history] = registry.get<const actor::DamageHistory>(entity);
				const auto last_info = damage_history.back();
				const auto last_attacker = last_info.attacker;

				helper::Actor::kill(registry, entity, last_attacker);
				helper::Room::drop(registry, entity);
			}
		}
	}
}
