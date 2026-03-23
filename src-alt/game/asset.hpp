// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <array>
#include <string_view>

namespace pd::game
{
	// =========================================================
	// FONT
	// =========================================================

	enum class Fonts : std::uint8_t
	{
		// 主菜单字体
		MAIN_MENU = 0,

		COUNT,
	};

	constexpr std::array<std::string_view, std::to_underlying(Fonts::COUNT)> fonts
	{
			// MAIN_MENU
			R"(C:\Windows\Fonts\msyh.ttc)",
	};

	// =========================================================
	// TEXTURE
	// =========================================================

	// =========================================================
	// SOUND
	// =========================================================

	enum class Sounds : std::uint8_t
	{
		// 主菜单切换选项时音效
		MAIN_MENU_SWITCH_OPTION = 0,

		COUNT,
	};

	constexpr std::array<std::string_view, std::to_underlying(Sounds::COUNT)> sounds
	{
			// MAIN_MENU_SWITCH_OPTION
			"./media/sounds/main_menu_switch_option.ogg",
	};

	// =========================================================
	// MUSIC
	// =========================================================

	enum class Musics : std::uint8_t
	{
		// 启动游戏时音乐
		LAUNCH_GAME = 0,

		// 主菜单音乐
		MAIN_MENU,

		COUNT,
	};

	constexpr std::array<std::string_view, std::to_underlying(Musics::COUNT)> musics
	{
			// LAUNCH_GAME
			"./media/musics/launch_game.ogg",
			// MAIN_MENU
			"./media/musics/main_menu.ogg",
	};
}
