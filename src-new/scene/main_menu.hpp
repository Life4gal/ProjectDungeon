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
	class Main;
}

namespace pd::scene
{
	class MainMenu final : public Scene
	{
	public:
		using music_handler = manager::music_handler;

	private:
		std::unique_ptr<menu::Main> main_;

		// 主菜单音乐
		music_handler music_;

	public:
		explicit MainMenu() noexcept;

		MainMenu(const MainMenu&) noexcept = delete;
		auto operator=(const MainMenu&) noexcept -> MainMenu& = delete;

		// std::unique_ptr<menu::Main> --> menu::Main无完整定义
		MainMenu(MainMenu&&) noexcept;
		auto operator=(MainMenu&&) noexcept -> MainMenu&;
		~MainMenu() noexcept override;

		auto on_loaded() noexcept -> void override;

		auto on_initialized() noexcept -> void override;

		auto on_unloaded() noexcept -> void override;

		auto handle_event(const sf::Event& event) noexcept -> void override;

		auto update(sf::Time delta) noexcept -> void override;

		auto render(sf::RenderWindow& window) noexcept -> void override;
	};
}
