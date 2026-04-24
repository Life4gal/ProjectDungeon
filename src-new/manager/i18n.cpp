// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <manager/i18n.hpp>

#include <string>
#include <unordered_map>

#include <utility/string_hash.hpp>

namespace pd::manager
{
	namespace
	{
		std::unordered_map<std::string, std::string, utility::StringHash, std::equal_to<>> g_database // NOLINT(bugprone-throwing-static-initialization)
		{
				// 菜单 -- 主菜单
				{"MENU.MAIN.RANDOM_SEED", "种子"},
				{"MENU.MAIN.PLAYER_NAME", "玩家名"},
				{"MENU.MAIN.CONTINUE", "继续游戏"},
				{"MENU.MAIN.START", "开始新游戏"},
				{"MENU.MAIN.OPTION", "选项"},
				{"MENU.MAIN.QUIT", "退出"},
				// 游戏 -- 暂停菜单
				{"MENU.PAUSE.RESUME", "继续"},
				{"MENU.PAUSE.OPTION", "选项"},
				{"MENU.PAUSE.TO_MAIN_MENU", "返回主菜单"},
				{"MENU.PAUSE.TO_DESKTOP", "退出游戏"},
		};
	}

	auto Internationalization::map(const std::string_view key) noexcept -> std::string_view
	{
		const auto it = g_database.find(key);
		if (it == g_database.end())
		{
			return {};
		}

		return it->second;
	}
}
