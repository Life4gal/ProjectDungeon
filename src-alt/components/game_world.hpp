// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Time.hpp>

namespace pd::components::game_world
{
	// 此帧历时
	class FrameDelta final
	{
	public:
		sf::Time frame_delta;
	};

	// 从游戏开始到现在总历时
	class TotalElapsed final
	{
	public:
		sf::Time total_elapsed;
	};

	// 从游戏开始到现在游玩历时(暂停时停止计时)
	class PlayElapsed final
	{
	public:
		sf::Time play_elapsed;
	};

	// 标记当前游戏是否暂停
	class GamePaused final {};
}
