// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <components/room.hpp>

#include <events/room.hpp>
#include <events/door.hpp>

#include <SFML/System/Vector2.hpp>

namespace pd::systems
{
	class Room final
	{
	public:
		using position_type = sf::Vector2u;

		// =============================================
		// 事件响应
		// =============================================

		// 订阅事件
		static auto subscribe_events(entt::registry& registry) noexcept -> void;

		// 取消订阅事件
		static auto unsubscribe_events(entt::registry& registry) noexcept -> void;

	private:
		// 激活(进入)房间
		static auto on_active_room(entt::registry& registry, const events::room::Active& event) noexcept -> void;

		// 清理房间
		static auto on_clear_room(entt::registry& registry, const events::room::Cleared& event) noexcept -> void;

		// 禁用(离开)房间
		static auto on_inactive_room(entt::registry& registry, const events::room::Inactive& event) noexcept -> void;

		// 处理接触门实体事件
		static auto on_contact_door(entt::registry& registry, const events::door::Contacted& event) noexcept -> void;

		// 处理接触门感应器事件
		static auto on_contact_door_sensor(entt::registry& registry, const events::door::SensorContacted& event) noexcept -> void;

	public:
		// =============================================
		// 创建/销毁上下文
		// =============================================

		static auto create(entt::registry& registry) noexcept -> void;

		static auto destroy(entt::registry& registry) noexcept -> void;
	};
}
