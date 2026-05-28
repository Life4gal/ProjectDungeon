// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

namespace pd::helper
{
	class Door final
	{
	public:
		// 接触其他实体
		//
		// Event:
		//  event::door::Contacted
		static auto contact(entt::registry& registry, entt::entity door, entt::entity other) noexcept -> void;

		// 感应其他实体
		//
		// Event:
		//  event::door::Sensed
		//  event::room::Leave (Room::leave)
		//  event::room::Enter (Room::enter)
		//  event::camera::Translate (Camera::translate)
		static auto sense(entt::registry& registry, entt::entity door, entt::entity other) noexcept -> void;

		// 尝试开启指定的门
		//
		// Event:
		//  event::door::Opened
		static auto try_open(entt::registry& registry, entt::entity door) noexcept -> void;

		// 尝试关闭指定的门
		//
		// Event:
		//  event::door::Closed
		static auto try_close(entt::registry& registry, entt::entity door) noexcept -> void;
	};
}
