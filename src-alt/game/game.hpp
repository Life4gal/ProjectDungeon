// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <game/config.hpp>
#include <game/dispatcher.hpp>

#include <events/game.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

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
		GameConfig config_;
		Dispatcher dispatcher_;

		// 渲染窗口
		sf::RenderWindow window_;
		// 绝对时间时钟(不重启)
		sf::Clock absolute_clock_;
		// 更新计时器(每帧重置)
		sf::Clock clock_;

		// 游戏当前场景
		std::unique_ptr<scene_type> current_scene_;

		// ==============================
		// dispatcher绑定
		// ==============================

		auto on_request_scene_change(const events::RequestSceneChange& event) noexcept -> void;

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

		// 游戏配置
		[[nodiscard]] auto game_config() noexcept -> GameConfig&;

		// 游戏配置
		[[nodiscard]] auto game_config() const noexcept -> const GameConfig&;

		// 事件分发器
		[[nodiscard]] auto dispatcher() noexcept -> Dispatcher&;

		// 窗口大小
		[[nodiscard]] auto window_size() const noexcept -> sf::Vector2u;

		// 窗口宽度
		[[nodiscard]] auto window_width() const noexcept -> unsigned;

		// 窗口高度
		[[nodiscard]] auto window_height() const noexcept -> unsigned;

		// 游戏运行的绝对时间
		[[nodiscard]] auto time() const noexcept -> sf::Time;
	};
}
