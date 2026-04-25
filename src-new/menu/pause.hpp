// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <functional>

#include <menu/menu.hpp>

#include <manager/resource_fwd.hpp>

namespace pd::menu
{
	class Pause final : public Menu
	{
	public:
		using font_handler = manager::font_handler;
		using sound_handler = manager::sound_handler;

		enum class Option : std::uint8_t
		{
			// 恢复游戏
			RESUME = 0,
			// 选项
			OPTION,
			// 退出游戏到主菜单
			TO_MAIN_MENU,
			// 退出游戏
			TO_DESKTOP,

			COUNT,
		};

	private:
		// 暂停菜单字体
		font_handler font_;
		// 暂停菜单音效-切换选项
		sound_handler sound_switch_option_;

		// 选择的选项
		std::underlying_type_t<Option> selected_option_value_;

		// 引用外部的游戏暂停状态,用于在选项被确认时修改游戏暂停状态
		std::reference_wrapper<bool> pause_ref_;

		auto on_cursor_move(int x, int y) noexcept -> void override;
		auto on_cursor_click(int x, int y) noexcept -> void override;
		auto on_action(Action action) noexcept -> void override;
		auto on_update(sf::Time delta) noexcept -> void override;
		auto on_render(sf::RenderWindow& window) noexcept -> void override;

	public:
		explicit Pause(bool& pause) noexcept;

		// Pause(const Pause&) noexcept = delete;
		// auto operator=(const Pause&) noexcept -> Pause& = delete;
		//
		// Pause(Pause&&) noexcept = default;
		// auto operator=(Pause&&) noexcept -> Pause& = default;
		//
		// ~Pause() noexcept override;
	};
}
