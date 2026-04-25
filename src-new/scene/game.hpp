// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <memory>

#include <scene/scene.hpp>

#include <manager/resource_fwd.hpp>

namespace pd::menu
{
	class Pause;
}

namespace pd::scene
{
	class Game final : public Scene
	{
	public:
		using music_handler = manager::music_handler;

	private:
		std::unique_ptr<menu::Pause> pause_;

		// 游戏音乐
		music_handler music_;

		// ==================
		// 下面这几个变量原本属于world组件,为了它们编写一个相关的系统似乎有些鸡肋?暂时先将它们放在这里
		// ==================

		// 此帧历时
		sf::Time frame_delta_;
		// 从游戏开始到现在总历时
		sf::Time total_elapsed_;
		// 从游戏开始到现在游玩历时(暂停时停止计时)
		sf::Time play_elapsed_;
		// 当前游戏是否暂停
		bool is_paused_;

		// 开始一场游戏
		auto start_game() noexcept -> bool;

		// 从存档加载游戏
		auto load_game() noexcept -> bool;

		// 清理当前游戏并重新开始
		auto restart_game() noexcept -> void;

	public:
		explicit Game() noexcept;

		Game(const Game&) noexcept = delete;
		auto operator=(const Game&) noexcept -> Game& = delete;

		// std::unique_ptr<menu::Pause> --> menu::Pause无完整定义
		Game(Game&&) noexcept;
		auto operator=(Game&&) noexcept -> Game&;
		~Game() noexcept override;

		auto on_loaded() noexcept -> void override;

		auto on_initialized() noexcept -> void override;

		auto on_unloaded() noexcept -> void override;

		auto handle_event(const sf::Event& event) noexcept -> void override;

		auto update(sf::Time delta) noexcept -> void override;

		auto render(sf::RenderWindow& window) noexcept -> void override;
	};
}
