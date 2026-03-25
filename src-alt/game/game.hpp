// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <events/window.hpp>
#include <events/scene.hpp>

#include <prometheus/version-core.hpp>

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

#if PROMETHEUS_COMPILER_DEBUG

		// 下面这些变量不会使用,但是对调试有帮助

		manager::Random* debug_random_;
		manager::Font* debug_font_;
		manager::Texture* debug_texture_;
		manager::Sound* debug_sound_;
		manager::Music* debug_music_;
		manager::Event* debug_event_;
		manager::GameConfig* debug_game_config_;

#endif

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
