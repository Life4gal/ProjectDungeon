// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/door.hpp>

#include <component/door.hpp>

#include <entt/entt.hpp>
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
}
