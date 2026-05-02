// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/door.hpp>

#include <blueprint/door.hpp>

#include <component/door.hpp>
#include <component/state.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

namespace pd::listener::door
{
	namespace cd = component::door;
	namespace ed = event::door;

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
		SPDLOG_INFO("SensorBegin: [DOOR]=0x{:08X}, [OTHER]=0x{:08X}", entt::to_integral(sensor_begin.door), entt::to_integral(sensor_begin.other));
	}

	auto on_sensor_end([[maybe_unused]] entt::registry& registry, const ed::SensorEnd& sensor_end) noexcept -> void
	{
		SPDLOG_INFO("SensorEnd: [DOOR]=0x{:08X}, [OTHER]=0x{:08X}", entt::to_integral(sensor_end.door), entt::to_integral(sensor_end.other));
	}

	auto on_request_open(entt::registry& registry, [[maybe_unused]] const ed::RequestOpen& request_open) noexcept -> void
	{
		for (const auto view = registry.view<component::state::InCameraArea, cd::Door, cd::PhysicsShapeDoor>();
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
		for (const auto view = registry.view<component::state::InCameraArea, cd::Door, cd::PhysicsShapeDoor>();
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
