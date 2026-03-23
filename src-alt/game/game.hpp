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
		// 绝对时间时钟(不重启)
		sf::Clock absolute_clock_;
		// 更新计时器(每帧重置)
		sf::Clock clock_;

		// 实体世界
		entt::registry registry_;

		// 游戏场景
		std::vector<std::unique_ptr<scene_type>> scenes_;
		// 游戏退出场景
		scene_type* quit_game_scene_;
		// 游戏当前场景
		scene_type* current_scene_;

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

		// ====================================
		// 一些可能变动,但是不应该(没必要)储存到实体世界的变量,可以直接通过接口获取
		// ====================================

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
