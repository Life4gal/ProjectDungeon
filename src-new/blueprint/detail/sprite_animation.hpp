// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <blueprint/detail/sprite.hpp>

namespace pd::blueprint
{
	class SpriteAnimation final
	{
	public:
		std::vector<Sprite> frames;
		// 毫秒
		std::vector<int> durations_ms;

		// 是否循环播放
		bool looping;
		// 是否反向播放
		bool reversed;
	};
}
