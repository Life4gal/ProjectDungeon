// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/animation.hpp>

#include <SFML/System/Time.hpp>

namespace pd::components
{
	// 当前动画帧计时器
	class AnimationTimer final
	{
	public:
		// 当前帧总持续时间
		sf::Time duration;
		// 当前帧已持续时间
		sf::Time elapsed;
	};

	// 当前动画帧索引
	class AnimationIndex final
	{
	public:
		// 此动画共有多少帧
		std::size_t total;
		// 此动画当前处于第几帧
		std::size_t current;
	};

	// 当前动画
	class AnimationView final
	{
	public:
		blueprint::AnimationView animation;
	};

	// 标记当前动画是否循环
	// 如果不循环则不存在该数据
	class AnimationLooping final {};

	// 标记当前动画是否暂停
	// 如果不暂停则不存在该数据
	class AnimationPaused final {};

	// 标记当前动画已经结束(此数据可以避免那些非循环动画也参与更新)
	// 如果未结束则不存在该数据
	class AnimationEnded final {};
}
