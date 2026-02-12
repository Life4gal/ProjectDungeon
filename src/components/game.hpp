// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <type_traits>

namespace pd
{
	class Game;
}

namespace pd::components
{
	// 游戏实例
	// 用于访问一些会不断更新但是不应该每帧都设置的内容(例如游戏总运行时间)
	class Game final
	{
	public:
		std::reference_wrapper<const pd::Game> game;
	};
}
