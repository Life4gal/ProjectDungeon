// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <blueprint/detail/sprite.hpp>

namespace pd::blueprint
{
	// SpriteAnimation != vector<Sprite>
	// SpriteAnimation要求所有帧的大小&原点&时长相同,仅是纹理&位置不同
	class SpriteAnimation final
	{
	public:
		class Frame final
		{
		public:
			std::string texture;

			int x;
			int y;
		};

		std::vector<Frame> frames;

		int width;
		int height;
		int origin_x;
		int origin_y;
		// 毫秒
		int duration_ms;

		// 是否循环播放
		bool looping;
		// 是否反向播放
		bool reversed;
	};
}
