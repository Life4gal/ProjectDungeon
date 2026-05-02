// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/door.hpp>

#include <manager/event.hpp>

#include <blueprint/door.hpp>

#include <designer/room.hpp>

#include <event/room.hpp>
#include <event/camera.hpp>
#include <event/player.hpp>

#include <component/door.hpp>
#include <component/state.hpp>
#include <component/player.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

namespace pd::listener::door
{
	namespace cd = component::door;
	namespace ed = event::door;

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

	auto on_contact_begin([[maybe_unused]] entt::registry& registry, const ed::ContactBegin& contact_begin) noexcept -> void
	{
		SPDLOG_INFO("ContactBegin: [DOOR]=0x{:08X}, [OTHER]=0x{:08X}", entt::to_integral(contact_begin.door), entt::to_integral(contact_begin.other));
	}

	auto on_contact_end([[maybe_unused]] entt::registry& registry, const ed::ContactEnd& contact_end) noexcept -> void
	{
		SPDLOG_INFO("ContactEnd: [DOOR]=0x{:08X}, [OTHER]=0x{:08X}", entt::to_integral(contact_end.door), entt::to_integral(contact_end.other));
	}

	auto on_sensor_begin([[maybe_unused]] entt::registry& registry, const ed::SensorBegin& sensor_begin) noexcept -> void
	{
		using namespace event;
		using manager::Event;

		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<cd::Door, cd::Direction>(sensor_begin.door)));

		SPDLOG_INFO("SensorBegin: [DOOR]=0x{:08X}, [OTHER]=0x{:08X}", entt::to_integral(sensor_begin.door), entt::to_integral(sensor_begin.other));

		// 无视非玩家实体
		if (sensor_begin.other_type != blueprint::PhysicsShapeType::PLAYER or not registry.all_of<component::player::Player>(sensor_begin.other))
		{
			return;
		}

		// 离开房间事件
		Event::enqueue(room::Leave{});

		// 移动相机和玩家
		switch (const auto door_direction = registry.get<const cd::Direction>(sensor_begin.door);
			door_direction)
		{
			case component::door::Direction::NORTH:
			{
				Event::enqueue(camera::Translate{.x = 0, .y = -CameraOffsetY});
				Event::enqueue(player::Translate{.x = 0, .y = -PlayerOffsetY});

				break;
			}
			case component::door::Direction::SOUTH:
			{
				Event::enqueue(camera::Translate{.x = 0, .y = CameraOffsetY});
				Event::enqueue(player::Translate{.x = 0, .y = PlayerOffsetY});

				break;
			}
			case component::door::Direction::WEST:
			{
				Event::enqueue(camera::Translate{.x = -CameraOffsetX, .y = 0});
				Event::enqueue(player::Translate{.x = -PlayerOffsetX, .y = 0});

				break;
			}
			case component::door::Direction::EAST:
			{
				Event::enqueue(camera::Translate{.x = CameraOffsetX, .y = 0});
				Event::enqueue(player::Translate{.x = PlayerOffsetX, .y = 0});

				break;
			}
			default: // NOLINT(clang-diagnostic-covered-switch-default)
			{
				PROMETHEUS_PLATFORM_UNREACHABLE();
			}
		}
	}

	auto on_sensor_end([[maybe_unused]] entt::registry& registry, const ed::SensorEnd& sensor_end) noexcept -> void
	{
		using namespace event;
		using manager::Event;

		PROMETHEUS_PLATFORM_ASSUME((registry.all_of<cd::Door, cd::Direction>(sensor_end.door)));

		SPDLOG_INFO("SensorEnd: [DOOR]=0x{:08X}, [OTHER]=0x{:08X}", entt::to_integral(sensor_end.door), entt::to_integral(sensor_end.other));

		// 无视非玩家实体
		if (sensor_end.other_type != blueprint::PhysicsShapeType::PLAYER or not registry.all_of<component::player::Player>(sensor_end.other))
		{
			return;
		}

		// 进入门感应区 --> SensorBegin --> 离开房间&移动相机&移动玩家 --> SensorEnd
		// 我们可以信赖此条调用链吗?

		// 进入房间
		Event::enqueue(room::Enter{});
	}

	auto on_request_open(entt::registry& registry, [[maybe_unused]] const ed::RequestOpen& request_open) noexcept -> void
	{
		using namespace component;

		for (const auto view = registry.view<state::InCameraArea, cd::Door, cd::PhysicsShapeDoor>();
		     const auto [entity, physics_shape_door]: view.each())
		{
			// physics
			{
				auto filter = b2Shape_GetFilter(physics_shape_door.shape);
				filter.maskBits = blueprint::PhysicsShapeCollisionMask::door_open;

				b2Shape_SetFilter(physics_shape_door.shape, filter);
			}

			// TODO: animation
		}
	}

	auto on_request_close(entt::registry& registry, [[maybe_unused]] const ed::RequestClose& request_close) noexcept -> void
	{
		using namespace component;

		for (const auto view = registry.view<state::InCameraArea, cd::Door, cd::PhysicsShapeDoor>();
		     const auto [entity, physics_shape_door]: view.each())
		{
			// physics
			{
				auto filter = b2Shape_GetFilter(physics_shape_door.shape);
				filter.maskBits = blueprint::PhysicsShapeCollisionMask::door_close;

				b2Shape_SetFilter(physics_shape_door.shape, filter);
			}

			// TODO: animation
		}
	}
}
