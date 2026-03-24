// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/core/hashed_string.hpp>

namespace pd::components::tags
{
	using entt::literals::operator ""_hs;

	// =================================================
	// 身份标识
	// =================================================

	// =================
	// 房间

	using floor = entt::tag<"Floor"_hs>;
	using wall = entt::tag<"Wall"_hs>;
	using obstacle = entt::tag<"Obstacle"_hs>;
	using hole = entt::tag<"Hole"_hs>;
	using door = entt::tag<"Door"_hs>;
	using door_sensor = entt::tag<"DoorSensor"_hs>;

	// =================================================
	// 状态标识
	// =================================================

	using disabled = entt::tag<"Disabled"_hs>;

	using dying = entt::tag<"Dying"_hs>;
	using dead = entt::tag<"Dead"_hs>;
}
