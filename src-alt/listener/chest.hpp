// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <events/chest.hpp>

#include <entt/fwd.hpp>

namespace pd::listener
{
	class Chest final
	{
	public:
		// 订阅事件
		static auto subscribe(entt::registry& registry) noexcept -> void;

		// 取消订阅事件
		static auto unsubscribe(entt::registry& registry) noexcept -> void;

	private:
		static auto on_disable(entt::registry& registry, const events::chest::Disable& event) noexcept -> void;

		static auto on_enable(entt::registry& registry, const events::chest::Enable& event) noexcept -> void;

		static auto on_disable_all(entt::registry& registry, const events::chest::DisableAll& event) noexcept -> void;

		static auto on_enable_all(entt::registry& registry, const events::chest::EnableAll& event) noexcept -> void;

		static auto on_contact(entt::registry& registry, const events::chest::Contacted& event) noexcept -> void;
	};
}
