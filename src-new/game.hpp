// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <optional>

#include <event/window.hpp>
#include <event/scene.hpp>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace sf
{
	class RenderWindow;
}

namespace pd
{
	namespace scene
	{
		class Scene;
	}

	class Game final
	{
	public:
		using scene_type = scene::Scene;

	private:
		// 渲染窗口
		sf::RenderWindow window_;
		// 更新计时器(每帧重置)
		sf::Clock clock_;

		// 游戏当前场景
		std::unique_ptr<scene_type> current_scene_;
		// 待切换的场景
		std::optional<scene::Category> pending_scene_;

		// 游戏窗口大小变化
		static auto on_window_resized(const event::window::Resized& event) noexcept -> void;
		// 游戏场景切换
		auto on_scene_switched(const event::scene::Switched& event) noexcept -> void;

	public:
		explicit Game(unsigned window_width, unsigned window_height, std::string_view title, bool fullscreen) noexcept;

		Game(const Game&) noexcept = delete;
		auto operator=(const Game&) noexcept -> Game& = delete;
		Game(Game&&) noexcept = delete;
		auto operator=(Game&&) noexcept -> Game& = delete;

		~Game() noexcept;

		auto run() noexcept -> void;
	};
}
