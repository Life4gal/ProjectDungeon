// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <events/floor.hpp>

#include <entt/fwd.hpp>

namespace pd::listener
{
	class Floor final
	{
	public:
		// 订阅事件
		static auto subscribe(entt::registry& registry) noexcept -> void;

		// 取消订阅事件
		static auto unsubscribe(entt::registry& registry) noexcept -> void;

	private:
		// 创建楼层
		static auto on_generate_request(entt::registry& registry, const events::floor::GenerateRequest& event) noexcept -> void;

		// 销毁楼层
		static auto on_destroy_request(entt::registry& registry, const events::floor::DestroyRequest& event) noexcept -> void;

		// 进入房间
		static auto on_enter_room(entt::registry& registry, const events::floor::Entered& event) noexcept -> void;

		// 清理房间
		static auto on_clear_room(entt::registry& registry, const events::floor::Cleared& event) noexcept -> void;

		// 离开房间
		static auto on_leave_room(entt::registry& registry, const events::floor::Left& event) noexcept -> void;
	};
}
