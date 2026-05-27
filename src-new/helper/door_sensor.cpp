// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/door_sensor.hpp>

#include <manager/event.hpp>

#include <designer/room.hpp>

#include <event/door.hpp>

#include <component/door_sensor.hpp>

#include <helper/room.hpp>
#include <helper/camera.hpp>
#include <helper/player_controller.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>

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

	auto DoorSensor::sense(entt::registry& registry, const entt::entity door, const entt::entity other) noexcept -> void
	{
		// 无视非玩家实体
		if (not registry.all_of<tags::Player>(other))
		{
			return;
		}

		manager::Event::enqueue(event::door::Sensed{.door = door, .player = other});

		const auto door_direction = registry.get<const door_sensor::Direction>(door);
		const auto [room] = registry.get<const door_sensor::Room>(door);
		const auto [target_room] = registry.get<const door_sensor::TargetRoom>(door);

		Room::leave(registry, room);

		switch (door_direction)
		{
			case door_sensor::Direction::NORTH:
			{
				PlayerController::translate(registry, {0, -PlayerOffsetY});
				Camera::translate(registry, {0, -CameraOffsetY});

				break;
			}
			case door_sensor::Direction::SOUTH:
			{
				PlayerController::translate(registry, {0, PlayerOffsetY});
				Camera::translate(registry, {0, CameraOffsetY});

				break;
			}
			case door_sensor::Direction::WEST:
			{
				PlayerController::translate(registry, {-PlayerOffsetX, 0});
				Camera::translate(registry, {-CameraOffsetX, 0});

				break;
			}
			case door_sensor::Direction::EAST:
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
}
