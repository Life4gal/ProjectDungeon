// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/core/type_traits.hpp>
#include <entt/core/hashed_string.hpp>

namespace pd::components::tags
{
	using entt::literals::operator""_hs;

	// 墙壁标识符
	using wall = entt::tag<"Wall"_hs>;

	// 地板标识符
	using floor = entt::tag<"Floor"_hs>;

	// 装饰物标识符
	using decoration = entt::tag<"Decoration"_hs>;

	// 触发器标识符
	using trigger = entt::tag<"Trigger"_hs>;

	// 钥匙标识符
	using key = entt::tag<"Key"_hs>;

	// 门标识符
	using door = entt::tag<"Door"_hs>;

	// 玩家标识符
	using player = entt::tag<"Player"_hs>;

	// 敌人标识符
	using enemy = entt::tag<"Enemy"_hs>;
}
