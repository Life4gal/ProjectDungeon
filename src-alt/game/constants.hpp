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
	constexpr std::array /*<sf::Keyboard::Key, std::to_underlying(MenuAction::COUNT)>*/ MenuActionKeys
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

	constexpr std::array<std::string_view, std::to_underlying(MainMenuOption::COUNT)> MainMenuOptionNames
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
	constexpr float MainMenuBeginX = 290.f;
	constexpr float MainMenuBeginY = 300.f;
	// 主菜单选项字体大小
	constexpr int MainMenuFontSize = 24;
	// 主菜单选项高度
	constexpr float MainMenuOptionHeight = static_cast<float>(MainMenuFontSize) * 1.5f;
	// 主菜单选项颜色
	constexpr sf::Color MainMenuOptionColorNormal{120, 120, 120, 255};
	constexpr sf::Color MainMenuOptionColorSelected{255, 255, 255, 255};

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

	constexpr std::array<std::string_view, std::to_underlying(GamePauseMenuOption::COUNT)> GamePauseMenuOptionNames
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
		// 游戏内HUD字体
		GAME_HUD,
		// 游戏暂停菜单字体
		GAME_PAUSE_MENU,

		COUNT,
	};

	constexpr std::array<std::string_view, std::to_underlying(Font::COUNT)> FontPaths
	{
			// MAIN_MENU
			R"(C:\Windows\Fonts\msyh.ttc)",
			// GAME_HUD
			R"(C:\Windows\Fonts\msyh.ttc)",
			// GAME_PAUSE_MENU
			R"(C:\Windows\Fonts\msyh.ttc)",
	};

	// ====================
	// 纹理资源
	// ====================

	enum class Texture : std::uint8_t
	{
		// 墙壁
		WALL,
		// 地板
		FLOOR,

		COUNT,
	};

	constexpr std::array<std::string_view, std::to_underlying(Texture::COUNT)> TexturePaths
	{
			// WALL
			R"(.\media\textures\TILES-WALL.png)",
			// FLOOR
			R"(.\media\textures\TILES-FLOOR.png)",
	};

	// ========
	// 墙壁

	// 墙类型(总共八行)
	constexpr std::uint32_t TextureTileWallCategory = 8;
	// 一行10列
	constexpr std::uint32_t TextureTileWallCountPerRow = 10;
	// 墙角
	constexpr std::uint32_t TextureTileWallCornerOffset = 0;
	constexpr std::uint32_t TextureTileWallInnerCornerOffset = 1;
	// 非墙角
	constexpr std::uint32_t TextureTileWallOffset = 2;
	constexpr std::uint32_t TextureTileWallCount = 8;

	// ====================
	// 音效资源
	// ====================

	enum class Sound : std::uint8_t
	{
		// 主菜单切换选项时音效
		MENU_SWITCH_OPTION = 0,

		COUNT,
	};

	constexpr std::array<std::string_view, std::to_underlying(Sound::COUNT)> SoundPaths
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

	constexpr std::array<std::string_view, std::to_underlying(Music::COUNT)> MusicPaths
	{
			// LAUNCH_GAME
			R"(.\media\musics\launch_game.wav)",
			// MAIN_MENU
			R"(.\media\musics\main_menu.wav)",
			// GAME
			R"(.\media\musics\game.wav)",
	};

	// =========================================
	// 地下城 -- 房间 -- 门
	// =========================================

	// 门物理体占整个瓦片的比例
	constexpr float DoorBodyRatio = 0.6667f;
	// 门感应区占整个瓦片的比例
	constexpr float DoorSensorRatio = 0.3f;
	// 门阻挡占整个瓦片的比例(防止玩家走到地图外部)
	constexpr float DoorBlockerRatio = 1 - DoorBodyRatio - DoorSensorRatio;

	// =========================================
	// 地下城 -- 房间
	// =========================================

	// 房间水平方向的网格数量
	constexpr std::uint32_t RoomHorizontalGrid = 15;
	// 房间垂直方向的网格数量
	constexpr std::uint32_t RoomVerticalGrid = 9;

	// 房间一个网格的宽度
	constexpr std::uint32_t RoomTileWidth = 64;
	// 房间一个网格的高度
	constexpr std::uint32_t RoomTileHeight = 64;

	// 房间的宽度
	constexpr std::uint32_t RoomWidth = RoomHorizontalGrid * RoomTileWidth;
	// 房间的高度
	constexpr std::uint32_t RoomHeight = RoomVerticalGrid * RoomTileHeight;

	// =========================================
	// 地下城 -- 楼层
	// =========================================

	// 地下城一层的宽度(以房间数量表示)
	constexpr std::uint32_t FloorHorizontalRoom = 13;
	// 地下城一层的高度(以房间数量表示)
	constexpr std::uint32_t FloorVerticalRoom = 7;

	// 地下城一层的起始房间坐标X
	constexpr std::uint32_t FloorStartRoomX = FloorHorizontalRoom / 2;
	// 地下城一层的起始房间坐标Y
	constexpr std::uint32_t FloorStartRoomY = FloorVerticalRoom / 2;

	// 地下城一层的房间数量基数
	constexpr std::uint32_t FloorRoomBaseCount = 10;
	// 地下城一层的房间数量增长系数(level * factor)
	constexpr std::uint32_t FloorRoomCountGrowthFactor = 4;
	// 地下城一层的房间数量最大值
	constexpr std::uint32_t FloorRoomMaxCount = FloorRoomBaseCount + FloorRoomCountGrowthFactor * 10;

	static_assert(FloorRoomMaxCount < FloorHorizontalRoom * FloorVerticalRoom);

	// =========================================
	// 地下城
	// =========================================

	// 
}
