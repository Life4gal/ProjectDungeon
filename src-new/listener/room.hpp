// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <event/room.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::listener::room
{
	auto on_leave(entt::registry& registry, const event::room::Leave& leave) noexcept -> void;

	auto on_enter(entt::registry& registry, const event::room::Enter& enter) noexcept -> void;
}
