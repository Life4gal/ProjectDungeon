// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <event/player.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::listener::player
{
	auto on_move_to(entt::registry& registry, const event::player::MoveTo& move_to) noexcept -> void;

	auto on_translate(entt::registry& registry, const event::player::Translate& translate) noexcept -> void;
}
