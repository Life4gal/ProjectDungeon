// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

namespace pd::helper
{
	class Room final
	{
	public:
		// 将一个敌人实体从房间的敌人列表中移除(仅此而已,并不会销毁实体)
		// 如果房间的敌人列表被清空则打开房间所有门
		//
		// Event:
		//  event::room::Cleared
		//  event::door::Opened (Door::try_open)
		static auto drop(entt::registry& registry, entt::entity enemy) noexcept -> void;

		// 开启房间所有门
		//
		// Event:
		//  event::door::Opened (Door::try_open)
		static auto open_doors(entt::registry& registry, entt::entity room) noexcept -> void;

		// 关闭房间所有门
		//
		// Event:
		//  event::door::Closed (Door::try_close)
		static auto close_doors(entt::registry& registry, entt::entity room) noexcept -> void;

		// 离开指定房间
		//
		// Event:
		//  event::room::Leave
		static auto leave(entt::registry& registry, entt::entity room) noexcept -> void;

		// 进入指定房间
		//
		// Event:
		//  event::room::Enter
		//  event::door::Closed (Door::try_close)
		static auto enter(entt::registry& registry, entt::entity room) noexcept -> void;
	};
}
