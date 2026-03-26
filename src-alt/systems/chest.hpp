// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <events/room.hpp>

namespace pd::systems
{
	class Chest final
	{
	public:
		// =============================================
		// 系统创建/销毁
		// =============================================

		// 事件订阅
		static auto create(entt::registry& registry) noexcept -> void;

		// 取消事件订阅
		static auto destroy(entt::registry& registry) noexcept -> void;

		// =============================================
		// 事件响应
		// =============================================

		// 处理接触宝箱事件
		static auto on_contact(entt::registry& registry, const events::ChestContacted& event) noexcept -> void;

		// =============================================
		// 实体生成
		// =============================================

		// 生成宝箱
		// todo
		static auto spawn(entt::registry& registry) noexcept -> entt::entity;
	};
}
