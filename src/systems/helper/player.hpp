// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>

namespace sf
{
	class Event;
}

namespace pd::systems::helper
{
	class Player final
	{
	public:
		// 在指定位置生成玩家
		static auto spawn(
			entt::registry& registry,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1},
			sf::Angle rotation = sf::degrees(0)
		) noexcept -> entt::entity;

		// 销毁玩家实体
		static auto kill(entt::registry& registry) noexcept -> void;

		// 处理事件
		static auto handle_event(entt::registry& registry, const sf::Event& event) noexcept -> void;

		// 应用玩家移动
		static auto apply_movement(entt::registry& registry) noexcept -> void;

	private:
		// 获取当前玩家实体
		[[nodiscard]] static auto get(entt::registry& registry) noexcept -> entt::entity;
	};
}
