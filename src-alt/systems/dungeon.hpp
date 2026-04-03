// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <events/dungeon.hpp>

#include <entt/fwd.hpp>

namespace pd::systems
{
	class Dungeon final
	{
	public:
		// =============================================
		// 事件响应
		// =============================================

		// 订阅事件
		static auto subscribe_events(entt::registry& registry) noexcept -> void;

		// 取消订阅事件
		static auto unsubscribe_events(entt::registry& registry) noexcept -> void;

	private:
		// 进入地下城
		static auto on_go(entt::registry& registry, const events::dungeon::Go& event) noexcept -> void;

	public:
		// =============================================
		// 创建/销毁上下文
		// =============================================

		static auto create(entt::registry& registry) noexcept -> void;

		static auto destroy(entt::registry& registry) noexcept -> void;
	};
}
