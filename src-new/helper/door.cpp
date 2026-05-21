// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/door.hpp>

#include <manager/event.hpp>

#include <blueprint/door.hpp>

#include <designer/room.hpp>

#include <event/door.hpp>

#include <component/door.hpp>

#include <helper/room.hpp>
#include <helper/camera.hpp>
#include <helper/player_controller.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <box2d/box2d.h>
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

	auto Door::contact([[maybe_unused]] entt::registry& registry, const entt::entity door, const entt::entity other) noexcept -> void
	{
		SPDLOG_INFO(
			"门实体(0x{:08X})与实体(0x{:08X})接触!",
			entt::to_integral(door),
			entt::to_integral(other)
		);

		manager::Event::enqueue(event::door::Contacted{.door = door, .other = other});
	}

	auto Door::sense(entt::registry& registry, const entt::entity door, const entt::entity other) noexcept -> void
	{
		// 无视非玩家实体
		if (not registry.all_of<tags::Player>(other))
		{
			return;
		}

		const auto door_direction = registry.get<const door::Direction>(door);
		const auto [room] = registry.get<const door::Room>(door);
		const auto [target_room] = registry.get<const door::TargetRoom>(door);

		Room::leave(registry, room);

		switch (door_direction)
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

		Room::enter(registry, target_room);
	}

	auto Door::try_open(entt::registry& registry, const entt::entity door) noexcept -> void
	{
		// TODO: 如果门需要钥匙,检测玩家是否拥有钥匙?

		// physics
		if (const auto* physics_shape_door = registry.try_get<const door::PhysicsShapeDoor>(door))
		{
			auto filter = b2Shape_GetFilter(physics_shape_door->shape);
			filter.maskBits = blueprint::CollisionMask::door_open;

			b2Shape_SetFilter(physics_shape_door->shape, filter);
		}
		else
		{
			SPDLOG_WARN("门实体(0x{:08X})找不到物理体组件!这可能导致门无法通过!", entt::to_integral(door));
			return;
		}

		// TODO: animation

		manager::Event::enqueue(event::door::Opened{.door = door});
	}

	auto Door::try_close(entt::registry& registry, const entt::entity door) noexcept -> void
	{
		// physics
		if (const auto* physics_shape_door = registry.try_get<const door::PhysicsShapeDoor>(door))
		{
			auto filter = b2Shape_GetFilter(physics_shape_door->shape);
			filter.maskBits = blueprint::CollisionMask::door_close;

			b2Shape_SetFilter(physics_shape_door->shape, filter);
		}
		else
		{
			SPDLOG_WARN("门实体(0x{:08X})找不到物理体组件!这可能导致门无法阻止通过!", entt::to_integral(door));
			return;
		}

		// TODO: animation

		manager::Event::enqueue(event::door::Closed{.door = door});
	}
}
