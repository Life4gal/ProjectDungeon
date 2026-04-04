// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <events/door.hpp>
#include <events/room.hpp>

#include <components/door.hpp>

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>

namespace pd::systems
{
	class Door final
	{
	public:
		using direction_type = components::door::Direction;
		using type_type = components::door::Type;

		// =============================================
		// 事件响应
		// =============================================

		// 订阅事件
		static auto subscribe_events(entt::registry& registry) noexcept -> void;

		// 取消订阅事件
		static auto unsubscribe_events(entt::registry& registry) noexcept -> void;

	private:
		static auto on_disable(entt::registry& registry, const events::room::DisableDoor& event) noexcept -> void;

		static auto on_enable(entt::registry& registry, const events::room::EnableDoor& event) noexcept -> void;

		static auto on_open_request(entt::registry& registry, const events::door::OpenRequest& event) noexcept -> void;

		static auto on_close_request(entt::registry& registry, const events::door::CloseRequest& event) noexcept -> void;

		static auto on_contact_door(entt::registry& registry, const events::door::Contacted& event) noexcept -> void;

		static auto on_contact_door_sensor(entt::registry& registry, const events::door::SensorContacted& event) noexcept -> void;

	public:
		// =============================================
		// 创建/销毁实体
		// =============================================

		// 创建门
		static auto spawn(entt::registry& registry, sf::Vector2f position, direction_type direction, type_type type) noexcept -> entt::entity;

		// 销毁门
		static auto destroy(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 销毁所有门
		static auto destroy_all(entt::registry& registry) noexcept -> void;
	};
}
