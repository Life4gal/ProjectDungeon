// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::trigger
{
	class Room final
	{
	public:
		// 检测目标房间是否已清理(即房间内没有敌人了),如果已清理则打开房间所有门
		// 返回房间是否已清理
		//
		// Event:
		//  event::room::Cleared
		//  event::door::Opened (Door::try_open)
		//
		// TODO: 当前的房间缺少一种机制,允许我们在房间被清理(且门已打开)后,(触发某个机关)往房间中再次投放敌人(且关闭房门)
		static auto check(entt::registry& registry, entt::entity room) noexcept -> bool;

		// 开启房间所有门
		// room: 目标房间
		//
		// Event:
		//  event::door::Opened (Door::try_open)
		static auto open_doors(entt::registry& registry, entt::entity room) noexcept -> void;

		// 关闭房间所有门
		// room: 目标房间
		//
		// Event:
		//  event::door::Closed (Door::try_close)
		static auto close_doors(entt::registry& registry, entt::entity room) noexcept -> void;

		// 离开指定房间
		// room: 指定房间
		// door: 从哪个门离开
		//
		// Event:
		//  event::room::Leave
		static auto leave(entt::registry& registry, entt::entity room, entt::entity door) noexcept -> void;

		// 进入指定房间(仅在初次进入房间时使用)
		// room: 目标房间
		//
		// Event:
		//  event::room::Enter
		//  event::camera::Translate (Camera::translate)
		//  event::door::Closed (Door::try_close)
		static auto enter(entt::registry& registry, entt::entity room) noexcept -> void;

		// 进入指定房间(仅在初次进入房间时使用)
		// x: 目标房间在关卡布局中的X坐标
		// y: 目标房间在关卡布局中的Y坐标
		//
		// Event:
		//  event::room::Enter
		//  event::camera::Translate (Camera::translate)
		//  event::door::Closed (Door::try_close)
		static auto enter(entt::registry& registry, unsigned x, unsigned y) noexcept -> void;

		// 进入指定房间
		// room: 目标房间(其实也可以只传入门实体?)
		// door: 从哪个门进入
		//
		// Event:
		//  event::room::Enter
		//  event::camera::Translate (Camera::translate)
		//  event::door::Closed (Door::try_close)
		static auto enter(entt::registry& registry, entt::entity room, entt::entity door) noexcept -> void;
	};
}
