// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/room_alive_check.hpp>

#include <algorithm>

#include <component/level.hpp>
#include <component/room.hpp>
#include <component/enemy.hpp>

#include <helper/property.hpp>
#include <helper/room.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::update
{
	using namespace component;

	auto room_alive_check(entt::registry& registry, [[maybe_unused]] sf::Time delta) noexcept -> void
	{
		const auto [room] = registry.ctx().get<level::Room>();
		const auto view = registry.view<state::InCameraArea, tags::Enemy, property::Health>();

		// 如果房间已清理
		if (helper::Room::check(registry, room))
		{
			// 理论上房间内不应该存在存活敌人了,如果存在则说明房间被错误地标记为已清理了,我们需要消灭所有存活敌人
			if (view.begin() != view.end())
			{
				const auto [layout_position] = registry.get<const room::LayoutPosition>(room);
				const auto [position] = registry.get<const room::Position>(room);
				const auto [size] = registry.get<const room::Size>(room);

				SPDLOG_WARN(
					"房间(实体: 0x{:08X}, 位置: ({:.0f}:{:.0f})[{}:{}], 大小: ({:.0f}:{:.0f}))内依然存在存活敌人,但是房间被标记为已清理!正在消灭所有存活敌人...",
					entt::to_integral(room),
					position.x,
					position.y,
					layout_position.x,
					layout_position.y,
					size.x,
					size.y
				);

				for (const auto [entity, health]: view.each())
				{
					if (const auto [enemy_room] = registry.get<const enemy::Room>(entity);
						enemy_room != room)
					{
						SPDLOG_WARN(
							"敌人(0x{:08X})所属房间为(0x{:08X}),但当前房间为(0x{:08X})",
							entt::to_integral(entity),
							entt::to_integral(enemy_room),
							entt::to_integral(room)
						);
					}

					// FIXME: 被谁杀死?房间?
					helper::Property::kill(registry, entity, room);
				}
			}
		}
		// 如果房间未清理
		else
		{
			for (const auto [entity, health]: view.each())
			{
				if (health.health <= 0)
				{
					if (const auto [enemy_room] = registry.get<const enemy::Room>(entity);
						enemy_room != room)
					{
						SPDLOG_WARN(
							"敌人(0x{:08X})所属房间为(0x{:08X}),但当前房间为(0x{:08X})",
							entt::to_integral(entity),
							entt::to_integral(enemy_room),
							entt::to_integral(room)
						);
					}

					// TODO: 受伤记录应该如何利用?理论上这可以用于数据统计,但是如果我们将这部分数据保存在实体上,在实体被销毁后数据将不复存在
					const auto& [damage_history] = registry.get<const property::DamageHistory>(entity);
					const auto last_info = damage_history.back();
					const auto last_attacker = last_info.attacker;

					helper::Property::kill(registry, entity, last_attacker);
				}
			}
		}
	}
}
