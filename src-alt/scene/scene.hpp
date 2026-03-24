// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/registry.hpp>

#include <SFML/System/Time.hpp>

namespace sf
{
	class Event;
	class RenderWindow;
}

namespace pd
{
	class Game;
}

namespace pd::scene
{
	class Scene
	{
	protected:
		// 实体世界
		entt::registry registry_;

	public:
		explicit Scene(Game& game) noexcept;

		Scene(const Scene&) noexcept = delete;
		auto operator=(const Scene&) noexcept -> Scene& = delete;

		Scene(Scene&&) noexcept = default;
		auto operator=(Scene&&) noexcept -> Scene& = default;

		virtual ~Scene() noexcept;

		// 载入场景(刚创建)
		virtual auto on_loaded() noexcept -> void = 0;

		// 初始化场景
		virtual auto on_initialized() noexcept -> void = 0;

		// 卸载场景
		virtual auto on_unloaded() noexcept -> void = 0;

		// 处理事件
		virtual auto handle_event(const sf::Event& event) noexcept -> void = 0;

		// 帧更新
		virtual auto update(sf::Time delta) noexcept -> void = 0;

		// 渲染
		virtual auto render(sf::RenderWindow& window) noexcept -> void = 0;
	};
}
