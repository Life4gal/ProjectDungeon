// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <game/constants.hpp>

namespace pd::game
{
	// =========================================
	// 菜单操作
	// =========================================

	[[nodiscard]] constexpr auto map(const sf::Keyboard::Key& key) noexcept -> MenuAction
	{
		for (std::size_t i = 0; i < std::to_underlying(MenuAction::COUNT); ++i)
		{
			if (key == menu_action_keys[i])
			{
				return static_cast<MenuAction>(i);
			}
		}
		return MenuAction::NONE;
	}

	// =========================================
	// 场景类型
	// =========================================

	//

	// =========================================
	// 主菜单
	// =========================================

	[[nodiscard]] constexpr auto map(const MainMenuOption option) noexcept -> std::string_view
	{
		return main_menu_option_names[std::to_underlying(option)];
	}

	// =========================================
	// 游戏暂停菜单
	// =========================================

	[[nodiscard]] constexpr auto map(const GamePauseMenuOption option) noexcept -> std::string_view
	{
		return game_pause_menu_option_names[std::to_underlying(option)];
	}

	// =========================================
	// 资源类型
	// =========================================

	// ====================
	// 字体资源
	// ====================

	[[nodiscard]] constexpr auto map(const Font font) noexcept -> std::string_view
	{
		return font_paths[std::to_underlying(font)];
	}

	// ====================
	// 纹理资源
	// ====================

	//

	// ====================
	// 音效资源
	// ====================

	[[nodiscard]] constexpr auto map(const Sound sound) noexcept -> std::string_view
	{
		return sound_paths[std::to_underlying(sound)];
	}

	// ====================
	// 音乐资源
	// ====================

	[[nodiscard]] constexpr auto map(const Music music) noexcept -> std::string_view
	{
		return music_paths[std::to_underlying(music)];
	}

	// =========================================
	// 地下城 -- 房间
	// =========================================

	[[nodiscard]] constexpr auto operator-(const DoorDirection direction) noexcept -> DoorDirection
	{
		return static_cast<DoorDirection>(std::to_underlying(direction) ^ 0b01);
	}

	[[nodiscard]] constexpr auto map(const DoorDirection direction) noexcept -> std::pair<int, int>
	{
		return room_door_position[std::to_underlying(direction)];
	}
}
