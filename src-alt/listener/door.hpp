// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <events/door.hpp>

#include <entt/fwd.hpp>

namespace pd::listener
{
	class Door final
	{
		static auto do_disable(entt::registry& registry, entt::entity entity) noexcept -> void;

		static auto do_enable(entt::registry& registry, entt::entity entity) noexcept -> void;

	public:
		// 订阅事件
		static auto subscribe(entt::registry& registry) noexcept -> void;

		// 取消订阅事件
		static auto unsubscribe(entt::registry& registry) noexcept -> void;

	private:
		static auto on_disable(entt::registry& registry, const events::door::Disable& event) noexcept -> void;

		static auto on_enable(entt::registry& registry, const events::door::Enable& event) noexcept -> void;

		static auto on_disable_all(entt::registry& registry, const events::door::DisableAll& event) noexcept -> void;

		static auto on_enable_all(entt::registry& registry, const events::door::EnableAll& event) noexcept -> void;

		static auto on_open_request(entt::registry& registry, const events::door::OpenRequest& event) noexcept -> void;

		static auto on_close_request(entt::registry& registry, const events::door::CloseRequest& event) noexcept -> void;

		static auto on_contact(entt::registry& registry, const events::door::Contacted& event) noexcept -> void;

		static auto on_contact_sensor(entt::registry& registry, const events::door::SensorContacted& event) noexcept -> void;
	};
}
