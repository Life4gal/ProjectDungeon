// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cstdint>

namespace pd::scene
{
	enum class Type : std::uint8_t
	{
		// 主菜单
		MAIN_MENU,
		// 主游戏场景
		GAME,
		// 退出场景(需要吗?)
		QUIT,

		COUNT
	};
}
