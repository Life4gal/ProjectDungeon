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

#include <prometheus/meta/enumeration.hpp>
#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

namespace pd::listener::door
{
	using namespace prometheus;

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

	auto on_contact_begin([[maybe_unused]] entt::registry& registry, const event::physics::ContactBegin& contact_begin) noexcept -> void
	{
		namespace door = component::door;

		// 不是门直接跳过
		if (contact_begin.a_type != blueprint::PhysicsShapeType::DOOR and contact_begin.b_type != blueprint::PhysicsShapeType::DOOR)
		{
			return;
		}
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<door::Door>(contact_begin.a) or registry.all_of<door::Door>(contact_begin.b));

		const auto a = contact_begin.a_type == blueprint::PhysicsShapeType::DOOR;

		const auto door_entity = a ? contact_begin.a : contact_begin.b;
		const auto other_entity = a ? contact_begin.b : contact_begin.a;
		const auto other_type = a ? contact_begin.b_type : contact_begin.a_type;

		SPDLOG_INFO(
			"ContactBegin: [DOOR]=0x{:08X}, [OTHER]=0x{:08X}({})",
			entt::to_integral(door_entity),
			entt::to_integral(other_entity),
			meta::name_of(other_type)
		);
	}

	auto on_contact_end([[maybe_unused]] entt::registry& registry, const event::physics::ContactEnd& contact_end) noexcept -> void
	{
		namespace door = component::door;

		// 不是门直接跳过
		if (contact_end.a_type != blueprint::PhysicsShapeType::DOOR and contact_end.b_type != blueprint::PhysicsShapeType::DOOR)
		{
			return;
		}
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<door::Door>(contact_end.a) or registry.all_of<door::Door>(contact_end.b));

		const auto a = contact_end.a_type == blueprint::PhysicsShapeType::DOOR;

		const auto door_entity = a ? contact_end.a : contact_end.b;
		const auto other_entity = a ? contact_end.b : contact_end.a;
		const auto other_type = a ? contact_end.b_type : contact_end.a_type;

		SPDLOG_INFO(
			"ContactEnd: [DOOR]=0x{:08X}, [OTHER]=0x{:08X}({})",
			entt::to_integral(door_entity),
			entt::to_integral(other_entity),
			meta::name_of(other_type)
		);
	}

	auto on_sensor_begin([[maybe_unused]] entt::registry& registry, const event::physics::SensorBegin& sensor_begin) noexcept -> void
	{
		namespace door = component::door;
		namespace player = component::player;

		using manager::Event;

		// 不是门直接跳过
		if (sensor_begin.sensor_type != blueprint::PhysicsShapeType::DOOR)
		{
			return;
		}
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<door::Door>(sensor_begin.sensor));

		SPDLOG_INFO(
			"SensorBegin: [DOOR]=0x{:08X}, [OTHER]=0x{:08X}({})",
			entt::to_integral(sensor_begin.sensor),
			entt::to_integral(sensor_begin.visitor),
			meta::name_of(sensor_begin.visitor_type)
		);

		// 无视非玩家实体
		if (sensor_begin.visitor_type != blueprint::PhysicsShapeType::PLAYER)
		{
			return;
		}
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<player::Player>(sensor_begin.visitor));

		const auto door_direction = registry.get<const door::Direction>(sensor_begin.sensor);
		const auto [room] = registry.get<const door::Room>(sensor_begin.sensor);

		// 离开房间事件
		Event::enqueue(event::room::Leave{.room = room});

		// 移动相机和玩家
		switch (door_direction)
		{
			case door::Direction::NORTH:
			{
				Event::enqueue(event::camera::Translate{.x = 0, .y = -CameraOffsetY});
				Event::enqueue(event::player::Translate{.x = 0, .y = -PlayerOffsetY});

				break;
			}
			case door::Direction::SOUTH:
			{
				Event::enqueue(event::camera::Translate{.x = 0, .y = CameraOffsetY});
				Event::enqueue(event::player::Translate{.x = 0, .y = PlayerOffsetY});

				break;
			}
			case door::Direction::WEST:
			{
				Event::enqueue(event::camera::Translate{.x = -CameraOffsetX, .y = 0});
				Event::enqueue(event::player::Translate{.x = -PlayerOffsetX, .y = 0});

				break;
			}
			case door::Direction::EAST:
			{
				Event::enqueue(event::camera::Translate{.x = CameraOffsetX, .y = 0});
				Event::enqueue(event::player::Translate{.x = PlayerOffsetX, .y = 0});

				break;
			}
			default: // NOLINT(clang-diagnostic-covered-switch-default)
			{
				PROMETHEUS_PLATFORM_UNREACHABLE();
			}
		}
	}

	auto on_sensor_end([[maybe_unused]] entt::registry& registry, const event::physics::SensorEnd& sensor_end) noexcept -> void
	{
		namespace door = component::door;
		namespace player = component::player;

		using manager::Event;

		// 不是门直接跳过
		if (sensor_end.sensor_type != blueprint::PhysicsShapeType::DOOR)
		{
			return;
		}
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<door::Door>(sensor_end.sensor));

		SPDLOG_INFO(
			"SensorEnd: [DOOR]=0x{:08X}, [OTHER]=0x{:08X}({})",
			entt::to_integral(sensor_end.sensor),
			entt::to_integral(sensor_end.visitor),
			meta::name_of(sensor_end.visitor_type)
		);

		// 无视非玩家实体
		if (sensor_end.visitor_type != blueprint::PhysicsShapeType::PLAYER)
		{
			return;
		}
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<player::Player>(sensor_end.visitor));

		// 进入门感应区 --> SensorBegin --> 离开房间&移动相机&移动玩家 --> SensorEnd
		// 我们可以信赖此条调用链吗?

		const auto [target_room] = registry.get<const door::TargetRoom>(sensor_end.sensor);

		// 进入房间
		Event::enqueue(event::room::Enter{.room = target_room});
	}

	auto on_request_open(entt::registry& registry, [[maybe_unused]] const event::door::RequestOpen& request_open) noexcept -> void
	{
		namespace door = component::door;

		// TODO: 如果门需要钥匙,检测玩家是否拥有钥匙?

		// physics
		if (const auto* physics_shape_door = registry.try_get<const door::PhysicsShapeDoor>(request_open.door))
		{
			auto filter = b2Shape_GetFilter(physics_shape_door->shape);
			filter.maskBits = blueprint::PhysicsShapeCollisionMask::door_open;

			b2Shape_SetFilter(physics_shape_door->shape, filter);
		}

		// TODO: animation
	}

	auto on_request_close(entt::registry& registry, [[maybe_unused]] const event::door::RequestClose& request_close) noexcept -> void
	{
		namespace door = component::door;

		// physics
		if (const auto* physics_shape_door = registry.try_get<const door::PhysicsShapeDoor>(request_close.door))
		{
			auto filter = b2Shape_GetFilter(physics_shape_door->shape);
			filter.maskBits = blueprint::PhysicsShapeCollisionMask::door_close;

			b2Shape_SetFilter(physics_shape_door->shape, filter);
		}

		// TODO: animation
	}
}
