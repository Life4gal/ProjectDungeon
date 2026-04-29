// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <event/camera.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::listener::camera
{
	auto on_set(entt::registry& registry, const event::camera::Set& set) noexcept -> void;

	auto on_move(entt::registry& registry, const event::camera::Move& move) noexcept -> void;

	auto on_resize(entt::registry& registry, const event::camera::Resize& resize) noexcept -> void;
}
