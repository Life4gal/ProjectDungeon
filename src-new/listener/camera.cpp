// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/camera.hpp>

#include <component/camera.hpp>

#include <entt/entt.hpp>

namespace pd::listener::camera
{
	namespace cc = component::camera;
	namespace ec = event::camera;

	auto on_move(entt::registry& registry, const ec::Move& move) noexcept -> void
	{
		registry.ctx().emplace<cc::Dirty>();

		registry.ctx().insert_or_assign(cc::Position{.position = {move.x, move.y}});
	}

	auto on_resize(entt::registry& registry, const ec::Resize& resize) noexcept -> void
	{
		registry.ctx().emplace<cc::Dirty>();

		registry.ctx().insert_or_assign(cc::Size{.size = {resize.width, resize.height}});
	}
}
