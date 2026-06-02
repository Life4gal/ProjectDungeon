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

#include <component/level.hpp>
#include <component/room.hpp>

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

	auto Room::check(entt::registry& registry, const entt::entity room) noexcept -> bool
	{
		auto* room_enemies = registry.try_get<room::Enemies>(room);
		// 不存在该组件无需再检查
		if (room_enemies == nullptr)
		{
			return true;
		}

		bool has_valid = false;

		// 将已失效的实体设置为null
		std::ranges::for_each(
			room_enemies->enemies,
			[&](entt::entity& enemy) noexcept -> void
			{
				if (enemy != entt::null)
				{
					if (registry.valid(enemy))
					{
						has_valid = true;
					}
					else
					{
						enemy = entt::null;
					}
				}
			}
		);

		// 检查是否所有敌人实体均已无效
		if (not has_valid)
		{
			const auto [layout_position] = registry.get<const room::LayoutPosition>(room);
			const auto [position] = registry.get<const room::Position>(room);
			const auto [size] = registry.get<const room::Size>(room);
			SPDLOG_INFO(
				"检测到房间(实体: 0x{:08X}, 位置: ({:.0f}:{:.0f})[{}:{}], 大小: ({:.0f}:{:.0f}))已被清理",
				entt::to_integral(room),
				position.x,
				position.y,
				layout_position.x,
				layout_position.y,
				size.x,
				size.y
			);

			// 移除敌人列表
			registry.remove<room::Enemies>(room);

			// 房间已清理
			manager::Event::enqueue(event::room::Cleared{.room = room});

			// 开门
			open_doors(registry, room);
		}

		return not has_valid;
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
			"离开房间(实体: 0x{:08X}, 位置: ({:.0f}:{:.0f})[{}:{}], 大小: ({:.0f}:{:.0f}))",
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

		// 上下文
		{
			// 设置上一个房间
			registry.ctx().insert_or_assign(level::LastRoom{.room = room});
		}
	}

	auto Room::enter(entt::registry& registry, const entt::entity room) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Room>(room));

		const auto [layout_position] = registry.get<const room::LayoutPosition>(room);
		const auto [position] = registry.get<const room::Position>(room);
		const auto [size] = registry.get<const room::Size>(room);
		SPDLOG_INFO(
			"进入房间(实体: 0x{:08X}, 位置: ({:.0f}:{:.0f})[{}:{}], 大小: ({:.0f}:{:.0f}))",
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
		// 注意要先移动相机再移动玩家,否则玩家的transform::ScreenPosition不准确
		{
			// 房间中心点
			const auto start_position = position + size / 2.0f;

			// TODO: 可视(相机)区域大小?
			Camera::initialize(registry, {position, size});
			PlayerController::move_to(registry, start_position);
		}

		// 上下文
		{
			// 记录路线
			auto& [path] = registry.ctx().get<level::Path>();
			path.push_back(room);

			// 设置当前房间
			// 上一个房间由leave设置
			registry.ctx().insert_or_assign(level::Room{.room = room});

			// 标记房间切换
			registry.ctx().emplace<level::RoomChanged>();
		}

		if (const auto* enemies = registry.try_get<room::Enemies>(room);
			enemies != nullptr)
		{
			// 房间未清理,关门
			close_doors(registry, room);
		}
	}

	auto Room::enter(entt::registry& registry, const unsigned x, const unsigned y) noexcept -> void
	{
		const auto& [position_to_entity] = registry.ctx().get<level::PositionToEntity>();
		const auto position = blueprint::LayoutPosition{.x = x, .y = y};

		const auto it = position_to_entity.find(position);
		if (it == position_to_entity.end())
		{
			// TODO: 如何处理?
			SPDLOG_ERROR("没有房间位于[{}:{}],无法进入房间", x, y);
			return;
		}

		const auto room = it->second;
		enter(registry, room);
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
			"进入房间(实体: 0x{:08X}, 位置: ({:.0f}:{:.0f})[{}:{}], 大小: ({:.0f}:{:.0f}))",
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

		// 移动 相机+玩家
		// 注意要先移动相机再移动玩家,否则玩家的transform::ScreenPosition不准确
		switch (const auto door_direction = registry.get<const door::Direction>(door);
			door_direction)
		{
			case door::Direction::NORTH:
			{
				Camera::translate(registry, {0, -CameraOffsetY});
				PlayerController::translate(registry, {0, -PlayerOffsetY});

				break;
			}
			case door::Direction::SOUTH:
			{
				Camera::translate(registry, {0, CameraOffsetY});
				PlayerController::translate(registry, {0, PlayerOffsetY});

				break;
			}
			case door::Direction::WEST:
			{
				Camera::translate(registry, {-CameraOffsetX, 0});
				PlayerController::translate(registry, {-PlayerOffsetX, 0});

				break;
			}
			case door::Direction::EAST:
			{
				Camera::translate(registry, {CameraOffsetX, 0});
				PlayerController::translate(registry, {PlayerOffsetX, 0});

				break;
			}
			default: // NOLINT(clang-diagnostic-covered-switch-default)
			{
				PROMETHEUS_PLATFORM_UNREACHABLE();
			}
		}

		// 上下文
		{
			// 记录路线
			auto& [path] = registry.ctx().get<level::Path>();
			path.push_back(room);

			// 设置当前房间
			// 上一个房间由leave设置
			registry.ctx().insert_or_assign(level::Room{.room = room});

			// 标记房间切换
			registry.ctx().emplace<level::RoomChanged>();
		}

		if (const auto* enemies = registry.try_get<room::Enemies>(room);
			enemies != nullptr)
		{
			// 房间未清理,关门
			close_doors(registry, room);
		}
	}
}
