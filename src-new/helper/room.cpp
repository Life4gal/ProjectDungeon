// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/room.hpp>

#include <algorithm>
#include <ranges>

#include <manager/event.hpp>

#include <designer/room.hpp>

#include <event/room.hpp>

#include <component/room.hpp>
#include <component/enemy.hpp>

#include <helper/door.hpp>
#include <helper/camera.hpp>
#include <helper/player_controller.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::helper
{
	using namespace component;

	namespace
	{
		constexpr auto RoomWidth = static_cast<float>(designer::Room::width);
		constexpr auto RoomHeight = static_cast<float>(designer::Room::height);
		constexpr auto TileWidth = static_cast<float>(designer::Room::tile_width);
		constexpr auto TileHeight = static_cast<float>(designer::Room::tile_height);

		constexpr auto CameraOffsetX = RoomWidth;
		constexpr auto CameraOffsetY = RoomHeight;
		constexpr auto PlayerOffsetX = 2.25f * TileWidth;
		constexpr auto PlayerOffsetY = 2.25f * TileHeight;
	}

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
			const auto [layout_position] = registry.get<const room::LayoutPosition>(room_entity->room);
			const auto [position] = registry.get<const room::Position>(room_entity->room);
			const auto [size] = registry.get<const room::Size>(room_entity->room);
			SPDLOG_INFO(
				"清理房间(0x{:08X}),位置: ({:.0f}:{:.0f})[{}:{}], 大小: ({:.0f}:{:.0f})",
				entt::to_integral(room_entity->room),
				position.x,
				position.y,
				layout_position.x,
				layout_position.y,
				size.x,
				size.y
			);

			// 房间已清理
			manager::Event::enqueue(event::room::Cleared{.room = room_entity->room});
			// 开门
			open_doors(registry, room_entity->room);
		}
	}

	auto Room::open_doors(entt::registry& registry, const entt::entity room) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Room>(room));

		// ReSharper disable once CppTooWideScopeInitStatement
		const auto [doors] = registry.get<const room::Doors>(room);
		for (const auto door: doors)
		{
			if (not registry.valid(door))
			{
				continue;
			}

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
			if (not registry.valid(door))
			{
				continue;
			}

			Door::try_close(registry, door);
		}
	}

	auto Room::leave(entt::registry& registry, const entt::entity room, [[maybe_unused]] const entt::entity door) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Room>(room));
		// 门所属房间必须一致
		PROMETHEUS_PLATFORM_ASSUME(registry.get<door::Room>(door).room == room);

		const auto [layout_position] = registry.get<const room::LayoutPosition>(room);
		const auto [position] = registry.get<const room::Position>(room);
		const auto [size] = registry.get<const room::Size>(room);
		SPDLOG_INFO(
			"离开房间(0x{:08X}),位置: ({:.0f}:{:.0f})[{}:{}], 大小: ({:.0f}:{:.0f})",
			entt::to_integral(room),
			position.x,
			position.y,
			layout_position.x,
			layout_position.y,
			size.x,
			size.y
		);

		// 离开房间事件
		manager::Event::enqueue(event::room::Leave{.room = room});

		// 离开房间时有什么值得做的吗?
		// 理论上房间应该已经被清理,需要做什么检查吗?
		registry.remove<room::Enemies>(room);
	}

	auto Room::enter(entt::registry& registry, const entt::entity room) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Room>(room));

		const auto [layout_position] = registry.get<const room::LayoutPosition>(room);
		const auto [position] = registry.get<const room::Position>(room);
		const auto [size] = registry.get<const room::Size>(room);
		SPDLOG_INFO(
			"进入房间(0x{:08X}),位置: ({:.0f}:{:.0f})[{}:{}], 大小: ({:.0f}:{:.0f})",
			entt::to_integral(room),
			position.x,
			position.y,
			layout_position.x,
			layout_position.y,
			size.x,
			size.y
		);

		// 进入房间事件
		manager::Event::enqueue(event::room::Enter{.room = room});

		// 移动 玩家+相机
		{
			// 房间中心点
			const auto start_position = position + size / 2.0f;

			PlayerController::move_to(registry, start_position);
			// TODO: 可视(相机)区域大小?
			Camera::initialize(registry, {position, size});
		}

		// 关门
		close_doors(registry, room);
	}

	auto Room::enter(entt::registry& registry, const entt::entity room, const entt::entity door) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Room>(room));
		// 门目标房间必须一致
		PROMETHEUS_PLATFORM_ASSUME(registry.get<door::TargetRoom>(door).room == room);

		const auto [layout_position] = registry.get<const room::LayoutPosition>(room);
		const auto [position] = registry.get<const room::Position>(room);
		const auto [size] = registry.get<const room::Size>(room);
		SPDLOG_INFO(
			"进入房间(0x{:08X}),位置: ({:.0f}:{:.0f})[{}:{}], 大小: ({:.0f}:{:.0f})",
			entt::to_integral(room),
			position.x,
			position.y,
			layout_position.x,
			layout_position.y,
			size.x,
			size.y
		);

		// 进入房间事件
		manager::Event::enqueue(event::room::Enter{.room = room});

		// 移动 玩家+相机
		switch (const auto door_direction = registry.get<const door::Direction>(door);
			door_direction)
		{
			case door::Direction::NORTH:
			{
				PlayerController::translate(registry, {0, -PlayerOffsetY});
				Camera::translate(registry, {0, -CameraOffsetY});

				break;
			}
			case door::Direction::SOUTH:
			{
				PlayerController::translate(registry, {0, PlayerOffsetY});
				Camera::translate(registry, {0, CameraOffsetY});

				break;
			}
			case door::Direction::WEST:
			{
				PlayerController::translate(registry, {-PlayerOffsetX, 0});
				Camera::translate(registry, {-CameraOffsetX, 0});

				break;
			}
			case door::Direction::EAST:
			{
				PlayerController::translate(registry, {PlayerOffsetX, 0});
				Camera::translate(registry, {CameraOffsetX, 0});

				break;
			}
			default: // NOLINT(clang-diagnostic-covered-switch-default)
			{
				PROMETHEUS_PLATFORM_UNREACHABLE();
			}
		}

		if (const auto* enemies = registry.try_get<room::Enemies>(room);
			enemies != nullptr)
		{
			// 房间未清理,关门
			close_doors(registry, room);
		}
	}
}
