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
		// 游戏字体
		GAME,

		COUNT,
	};

	class Font final
	{
	public:
		constexpr static std::array<std::string_view, std::to_underlying(Fonts::COUNT)> fonts
		{
				// MAIN_MENU
				R"(C:\Windows\Fonts\msyh.ttc)",
				// GAME
				R"(C:\Windows\Fonts\msyh.ttc)",
		};

		[[nodiscard]] constexpr static auto get(const Fonts font) noexcept -> std::string_view
		{
			return fonts[std::to_underlying(font)];
		}
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
		MENU_SWITCH_OPTION = 0,

		COUNT,
	};

	class Sound final
	{
	public:
		constexpr static std::array<std::string_view, std::to_underlying(Sounds::COUNT)> sounds
		{
				// MENU_SWITCH_OPTION
				"./media/sounds/menu_switch_option.ogg",
		};

		[[nodiscard]] constexpr static auto get(const Sounds sound) noexcept -> std::string_view
		{
			return sounds[std::to_underlying(sound)];
		}
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
		// 游戏音乐
		GAME,

		COUNT,
	};

	class Music final
	{
	public:
		constexpr static std::array<std::string_view, std::to_underlying(Musics::COUNT)> musics
		{
				// LAUNCH_GAME
				"./media/musics/launch_game.ogg",
				// MAIN_MENU
				"./media/musics/main_menu.ogg",
				// GAME
				"./media/musics/game.ogg",
		};

		[[nodiscard]] constexpr static auto get(const Musics music) noexcept -> std::string_view
		{
			return musics[std::to_underlying(music)];
		}
	};
}
