// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <entt/entity/registry.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

namespace pd
{
	class Scene;

	class Game final
	{
	public:

	private:
		// 渲染窗口
		sf::RenderWindow window_;
		// 计时器(绝对时间)
		sf::Clock clock_;

		// 实体世界
		entt::registry registry_;

		// 游戏场景
		std::vector<std::unique_ptr<Scene>> scenes_;
		// 游戏当前场景
		Scene* current_scene_;

	public:
		Game(
			unsigned window_width,
			unsigned window_height,
			std::string_view title,
			bool fullscreen
		) noexcept;

		Game(const Game&) noexcept = delete;
		auto operator=(const Game&) noexcept -> Game& = delete;
		Game(Game&&) noexcept = delete;
		auto operator=(Game&&) noexcept -> Game& = delete;

		~Game() noexcept;

		auto run() noexcept -> void;
	};
}
