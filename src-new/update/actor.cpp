// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/actor.hpp>

#include <manager/event.hpp>

#include <event/actor.hpp>

#include <component/actor.hpp>
#include <component/state.hpp>
#include <component/player.hpp>

#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;

	auto actor(entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		const auto view = registry
				.view<
					state::InCameraArea,
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

				// actor::Dead事件负责添加标签
				// TODO: 在这里添加标签也许性能更好?(因为state::Dead可能致使后续的一些逻辑被跳过)
				// registry.emplace<state::Dead>(entity);

				const auto& [damage_history] = registry.get<const actor::DamageHistory>(entity);
				const auto last_info = damage_history.back();
				const auto last_attacker = last_info.attacker;
				manager::Event::enqueue(event::actor::Dead{.attacker = last_attacker, .victim = entity});
			}
		}
	}
}
