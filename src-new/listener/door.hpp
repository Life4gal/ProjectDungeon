// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <event/physics.hpp>
#include <event/door.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::listener::door
{
	auto on_contact_begin(entt::registry& registry, const event::physics::ContactBegin& contact_begin) noexcept -> void;

	auto on_contact_end(entt::registry& registry, const event::physics::ContactEnd& contact_end) noexcept -> void;

	auto on_sensor_begin(entt::registry& registry, const event::physics::SensorBegin& sensor_begin) noexcept -> void;

	auto on_sensor_end(entt::registry& registry, const event::physics::SensorEnd& sensor_end) noexcept -> void;

	auto on_request_open(entt::registry& registry, const event::door::RequestOpen& request_open) noexcept -> void;

	auto on_request_close(entt::registry& registry, const event::door::RequestClose& request_close) noexcept -> void;
}
