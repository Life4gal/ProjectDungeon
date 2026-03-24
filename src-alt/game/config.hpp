// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>

namespace pd
{
    class GameConfig final
	{
	public:
		// 音效音量
		float sound_volume{100};
		// 音乐音量
		float music_volume{100};

		// 这决定我们最多能创建多少个sf::Sound实例
		// 这部分资源基本不会释放 :(
		std::uint32_t max_sound_channels{24};

		std::string player_name{"Player"};
	};
}
