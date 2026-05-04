// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/room.hpp>

#include <algorithm>

#include <manager/event.hpp>

#include <event/door.hpp>

#include <component/room.hpp>
#include <component/enemy.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::listener::room
{
	auto on_dead(entt::registry& registry, const event::actor::Dead& dead) noexcept -> void
	{
		namespace room = component::room;
		namespace enemy = component::enemy;

		if (not registry.all_of<enemy::Enemy>(dead.victim))
		{
			// 不是敌人不处理
			return;
		}

		const auto [room_entity] = registry.get<enemy::Room>(dead.victim);
		auto& [room_enemies] = registry.get<room::Enemies>(room_entity);

		if (const auto it = std::ranges::find(room_enemies, dead.victim);
			it == room_enemies.end())
		{
			SPDLOG_WARN("敌人(0x{:08X})属于房间(0x{:08X}),但是房间的敌人列表中未记录!");
		}
		else
		{
			// 设置为null
			*it = entt::null;
		}

		// 检查房间是否已清空
		if (std::ranges::all_of(
			room_enemies,
			[](const entt::entity entity) noexcept -> bool
			{
				return entity == entt::null;
			}
		))
		{
			// 开门
			// ReSharper disable once CppTooWideScopeInitStatement
			const auto [doors] = registry.get<const room::Doors>(room_entity);
			for (const auto door: doors)
			{
				manager::Event::enqueue(event::door::RequestOpen{.door = door});
			}
		}
	}

	auto on_leave(entt::registry& registry, const event::room::Leave& leave) noexcept -> void
	{
		namespace room = component::room;

		// 离开房间时有什么值得做的吗?
		// 理论上房间应该已经被清理,需要做什么检查吗?

		registry.remove<room::Enemies>(leave.room);
	}

	auto on_enter(entt::registry& registry, const event::room::Enter& enter) noexcept -> void
	{
		namespace room = component::room;

		if (const auto* enemies = registry.try_get<room::Enemies>(enter.room);
			enemies == nullptr)
		{
			// 房间已清理,什么都不做
			return;
		}

		// 房间未清理,关门
		// ReSharper disable once CppTooWideScopeInitStatement
		const auto [doors] = registry.get<const room::Doors>(enter.room);
		for (const auto door: doors)
		{
			manager::Event::enqueue(event::door::RequestClose{.door = door});
		}
	}
}
