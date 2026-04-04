// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <events/chest.hpp>
#include <events/room.hpp>

#include <entt/fwd.hpp>

namespace pd::systems
{
	class Chest final
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
		static auto on_disable(entt::registry& registry, const events::room::DisableChest& event) noexcept -> void;

		static auto on_enable(entt::registry& registry, const events::room::EnableChest& event) noexcept -> void;

		static auto on_contact_chest(entt::registry& registry, const events::chest::Contacted& contacted) noexcept -> void;

	public:
		// =============================================
		// 创建/销毁实体
		// =============================================

		// 创建宝箱
		// todo: 参数?
		static auto spawn(entt::registry& registry) noexcept -> entt::entity;

		// 销毁宝箱
		static auto destroy(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 销毁所有宝箱
		static auto destroy_all(entt::registry& registry) noexcept -> void;
	};
}
