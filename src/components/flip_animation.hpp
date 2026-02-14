// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <span>

#include <blueprint/flip_animation.hpp>

#include <SFML/System/Time.hpp>

namespace pd::components
{
	// 当前翻页动画帧计时器
	class FlipAnimationTimer final
	{
	public:
		// 当前帧总持续时间
		sf::Time duration;
		// 当前帧已持续时间
		sf::Time elapsed;
	};

	// 当前翻页动画帧索引
	class FlipAnimationIndex final
	{
	public:
		// 此动画共有多少帧
		std::size_t total;
		// 此动画当前处于第几帧
		std::size_t current;
	};

	// 当前翻页动画(直接引用蓝图数据)
	class FlipAnimationFrames final
	{
	public:
		blueprint::flip_animation_frames_view_type frames;
	};
}
