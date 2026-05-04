// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <event/actor.hpp>
#include <event/room.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::listener::room
{
	// 敌人死亡
	auto on_dead(entt::registry& registry, const event::actor::Dead& dead) noexcept -> void;

	// 离开房间
	auto on_leave(entt::registry& registry, const event::room::Leave& leave) noexcept -> void;

	// 进入房间
	auto on_enter(entt::registry& registry, const event::room::Enter& enter) noexcept -> void;
}
