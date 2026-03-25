// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <array>
#include <string_view>

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Color.hpp>

namespace pd::game
{
	// =========================================
	// 菜单操作
	// =========================================

	enum class MenuAction : std::uint8_t
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		CONFIRM,
		CANCEL,

		COUNT,
		NONE,
	};

	// FIXME: 可配置?
	constexpr std::array /*<sf::Keyboard::Key, std::to_underlying(MenuAction::COUNT)>*/ menu_action_keys
	{
			// UP
			sf::Keyboard::Key::Up,
			// DOWN
			sf::Keyboard::Key::Down,
			// LEFT
			sf::Keyboard::Key::Left,
			// RIGHT
			sf::Keyboard::Key::Right,
			// CONFIRM
			sf::Keyboard::Key::Enter,
			// CANCEL
			sf::Keyboard::Key::Escape,
	};

	// =========================================
	// 场景类型
	// =========================================

	enum class Scene : std::uint8_t
	{
		// 主菜单
		MAIN_MENU,
		// 主游戏场景
		GAME,
		// 退出场景(需要吗?)
		QUIT,

		COUNT
	};

	// =========================================
	// 主菜单
	// =========================================

	enum class MainMenuOption : std::uint8_t
	{
		// 随机数种子
		RANDOM_SEED = 0,
		// 玩家名称
		PLAYER_NAME,
		// 继续游戏
		CONTINUE,
		// 开始新游戏
		START,
		// 选项
		OPTIONS,
		// 退出游戏
		QUIT,

		COUNT
	};

	constexpr std::array<std::string_view, std::to_underlying(MainMenuOption::COUNT)> main_menu_option_names
	{
			// RANDOM_SEED
			// 不会使用
			"",
			// PLAYER_NAME
			// 作为前缀
			"玩家名: ",
			// CONTINUE
			"继续游戏",
			// START
			"开始新游戏",
			// OPTIONS
			"选项",
			// QUIT
			"退出"
	};

	// 主菜单选项开始位置
	constexpr float main_menu_begin_x = 290.f;
	constexpr float main_menu_begin_y = 300.f;
	// 主菜单选项字体大小
	constexpr int main_menu_font_size = 24;
	// 主菜单选项高度
	constexpr float main_menu_option_height = static_cast<float>(main_menu_font_size) * 1.5f;
	// 主菜单选项颜色
	constexpr sf::Color main_menu_option_color_normal{120, 120, 120, 255};
	constexpr sf::Color main_menu_option_color_selected{255, 255, 255, 255};

	// =========================================
	// 游戏暂停菜单
	// =========================================

	enum class GamePauseMenuOption : std::uint8_t
	{
		RESUME = 0,
		OPTIONS,
		QUIT,

		COUNT
	};

	constexpr std::array<std::string_view, std::to_underlying(GamePauseMenuOption::COUNT)> game_pause_menu_option_names
	{
			// RESUME
			"继续",
			// OPTIONS
			"选项",
			// QUIT
			"退出"
	};

	// =========================================
	// 资源类型
	// =========================================

	// ====================
	// 字体资源
	// ====================

	enum class Font : std::uint8_t
	{
		// 主菜单字体
		MAIN_MENU = 0,
		// 游戏字体
		GAME,

		COUNT,
	};

	constexpr std::array<std::string_view, std::to_underlying(Font::COUNT)> font_paths
	{
			// MAIN_MENU
			R"(C:\Windows\Fonts\msyh.ttc)",
			// GAME
			R"(C:\Windows\Fonts\msyh.ttc)",
	};

	// ====================
	// 纹理资源
	// ====================

	//

	// ====================
	// 音效资源
	// ====================

	enum class Sound : std::uint8_t
	{
		// 主菜单切换选项时音效
		MENU_SWITCH_OPTION = 0,

		COUNT,
	};

	constexpr std::array<std::string_view, std::to_underlying(Sound::COUNT)> sound_paths
	{
			// MENU_SWITCH_OPTION
			R"(.\media\sounds\menu_switch_option.ogg)",
	};

	// ====================
	// 音乐资源
	// ====================

	enum class Music : std::uint8_t
	{
		// 启动游戏时音乐
		LAUNCH_GAME = 0,
		// 主菜单音乐
		MAIN_MENU,
		// 游戏音乐
		GAME,

		COUNT,
	};

	constexpr std::array<std::string_view, std::to_underlying(Music::COUNT)> music_paths
	{
			// LAUNCH_GAME
			R"(.\media\musics\launch_game.wav)",
			// MAIN_MENU
			R"(.\media\musics\main_menu.wav)",
			// GAME
			R"(.\media\musics\game.wav)",
	};

	// =========================================
	// 地下城 -- 房间
	// =========================================

	enum class DoorDirection : std::uint8_t
	{
		NORTH = 0b00,
		SOUTH = 0b01,
		WEST = 0b10,
		EAST = 0b11,
	};

	constexpr std::uint32_t room_horizontal_grid = 15;
	constexpr std::uint32_t room_vertical_grid = 9;

	constexpr std::uint32_t room_tile_width = 64;
	constexpr std::uint32_t room_tile_height = 64;

	constexpr std::uint32_t room_width = room_horizontal_grid * room_tile_width;
	constexpr std::uint32_t room_height = room_vertical_grid * room_tile_height;

	constexpr std::uint32_t room_door_count = 4;
	constexpr std::array<std::pair<std::uint32_t, std::uint32_t>, room_door_count> room_door_position
	{{
			// NORTH
			{room_horizontal_grid / 2, 0},
			// SOUTH
			{room_horizontal_grid / 2, room_vertical_grid - 1},
			// WEST
			{0, room_vertical_grid / 2},
			// EAST
			{room_horizontal_grid - 1, room_vertical_grid / 2}
	}};
}
