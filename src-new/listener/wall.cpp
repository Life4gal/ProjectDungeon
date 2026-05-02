// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/wall.hpp>

#include <component/wall.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::listener::wall
{
	namespace cw = component::wall;
	namespace ew = event::wall;

	auto on_contact_begin([[maybe_unused]] entt::registry& registry, const ew::ContactBegin& contact_begin) noexcept -> void
	{
		SPDLOG_INFO("ContactBegin: [WALL]=0x{:08X}, [OTHER]=0x{:08X}", entt::to_integral(contact_begin.wall), entt::to_integral(contact_begin.other));
	}

	auto on_contact_end([[maybe_unused]] entt::registry& registry, const ew::ContactEnd& contact_end) noexcept -> void
	{
		SPDLOG_INFO("ContactEnd: [WALL]=0x{:08X}, [OTHER]=0x{:08X}", entt::to_integral(contact_end.wall), entt::to_integral(contact_end.other));
	}
}
