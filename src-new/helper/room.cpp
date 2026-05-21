// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/room.hpp>

#include <algorithm>
#include <ranges>

#include <manager/event.hpp>

#include <event/room.hpp>

#include <component/room.hpp>
#include <component/enemy.hpp>

#include <helper/door.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::helper
{
	using namespace component;

	auto Room::drop(entt::registry& registry, const entt::entity enemy) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Enemy>(enemy));

		const auto* room_entity = registry.try_get<enemy::Room>(enemy);
		if (room_entity == nullptr)
		{
			SPDLOG_WARN("尝试将实体(0x{:08X})从房间的敌人列表中移除,但是其不存在房间实体组件!也许不是敌人类型?", entt::to_integral(enemy));
			return;
		}

		auto& [room_enemies] = registry.get<room::Enemies>(room_entity->room);

		if (const auto it = std::ranges::find(room_enemies, enemy);
			it == room_enemies.end())
		{
			SPDLOG_WARN("尝试将实体(0x{:08X})从房间的敌人列表移除,但是其似乎不属于其记录的房间?", entt::to_integral(enemy));
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
			open_doors(registry, room_entity->room);

			manager::Event::enqueue(event::room::Cleared{.room = room_entity->room});
		}
	}

	auto Room::open_doors(entt::registry& registry, const entt::entity room) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Room>(room));

		// ReSharper disable once CppTooWideScopeInitStatement
		const auto [doors] = registry.get<const room::Doors>(room);
		for (const auto door: doors)
		{
			Door::try_open(registry, door);
		}
	}

	auto Room::close_doors(entt::registry& registry, const entt::entity room) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Room>(room));

		// ReSharper disable once CppTooWideScopeInitStatement
		const auto [doors] = registry.get<const room::Doors>(room);
		for (const auto door: doors)
		{
			Door::try_close(registry, door);
		}
	}

	auto Room::leave(entt::registry& registry, const entt::entity room) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Room>(room));

		const auto [position] = registry.get<const room::Position>(room);
		const auto [size] = registry.get<const room::Size>(room);
		SPDLOG_INFO("离开房间(0x{:08X}),位置: ({:.0f}:{:.0f}), 大小: ({:.0f}:{:.0f})", entt::to_integral(room), position.x, position.y, size.x, size.y);

		manager::Event::enqueue(event::room::Leave{.room = room});

		// 离开房间时有什么值得做的吗?
		// 理论上房间应该已经被清理,需要做什么检查吗?
		registry.remove<room::Enemies>(room);
	}

	auto Room::enter(entt::registry& registry, const entt::entity room) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Room>(room));

		const auto [position] = registry.get<const room::Position>(room);
		const auto [size] = registry.get<const room::Size>(room);
		SPDLOG_INFO("进入房间(0x{:08X}),位置: ({:.0f}:{:.0f}), 大小: ({:.0f}:{:.0f})", entt::to_integral(room), position.x, position.y, size.x, size.y);

		manager::Event::enqueue(event::room::Enter{.room = room});

		if (const auto* enemies = registry.try_get<room::Enemies>(room);
			enemies == nullptr)
		{
			// 房间已清理,什么都不做
			return;
		}

		// 房间未清理,关门
		close_doors(registry, room);
	}
}
