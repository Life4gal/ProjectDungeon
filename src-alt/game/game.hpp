// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <optional>

#include <events/window.hpp>
#include <events/scene.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

namespace pd
{
	namespace scene
	{
		class Scene;
	}

	namespace manager
	{
		class Random;
		class Font;
		class Texture;
		class Sound;
		class Music;
		class Event;
		class GameConfig;
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
		// 我们不在响应事件时创建场景实例,而是在Event::update之后再创建实例
		// 避免实例订阅事件影响Event::update
		std::optional<scene::Type> pending_scene_;

		// 游戏窗口大小变化
		auto on_window_resized(const events::WindowResized& event) noexcept -> void;
		// 游戏场景切换
		auto on_scene_changed(const events::SceneChanged& event) noexcept -> void;

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
