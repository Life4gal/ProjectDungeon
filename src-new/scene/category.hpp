// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cstdint>

namespace pd::scene
{
	enum class Category : std::uint8_t
	{
		// 主菜单
		MAIN_MENU,
		// 主游戏场景
		GAME,
		// 继续游戏场景(这个场景不存在,或者说这就是主游戏场景,只不过特别说明是"继续"游戏)
		GAME_CONTINUE,
		// 退出场景(需要吗?)
		QUIT,

		COUNT
	};
}
