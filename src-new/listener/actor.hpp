// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <event/actor.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::listener::actor
{
	auto on_hurt(entt::registry& registry, const event::actor::Hurt& hurt) noexcept -> void;

	auto on_dead(entt::registry& registry, const event::actor::Dead& dead) noexcept -> void;
}
