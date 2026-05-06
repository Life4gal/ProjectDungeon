// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

namespace pd::component::sprite_effect
{
	// 动画可以叠加,所以每种动画有自己的计时器
	// TODO: 当前特效仅允许每个种类(position/scale/color)同时一种生效

	// ============================================
	// 位置效果
	// ============================================

	namespace position
	{
		// // 平移
		// class Translate final
		// {
		// public:
		// 	// 动画开始时的位置偏移
		// 	sf::Vector2f from;
		// 	// 动画结束时的位置偏移
		// 	sf::Vector2f to;
		//
		// 	// 动画总持续时间
		// 	sf::Time duration;
		// 	// 动画已持续时间
		// 	sf::Time elapsed;
		// };
	}

	// ============================================
	// 大小效果
	// ============================================

	namespace scale
	{
		// 线性变化
		class Linear final
		{
		public:
			// 动画开始时的大小
			sf::Vector2f from;
			// 动画结束时的大小
			sf::Vector2f to;

			// 动画总持续时间
			sf::Time duration;
			// 动画已持续时间
			sf::Time elapsed;
		};

		// 冲击波变化
		class Shockwave final
		{
		public:
			// 基础大小
			sf::Vector2f base;
			// 峰值倍数
			float peak;

			// 动画总持续时间
			sf::Time duration;
			// 动画已持续时间
			sf::Time elapsed;
		};

		// 弹性变化
		class Bounce final
		{
		public:
			// 动画开始时的大小
			sf::Vector2f from;
			// 动画结束时的大小
			sf::Vector2f to;
			// 弹性系数(越小弹性越强)
			float stiffness;

			// 动画总持续时间
			sf::Time duration;
			// 动画已持续时间
			sf::Time elapsed;
		};

		// 呼吸变化
		class Breathing
		{
		public:
			// 基础大小
			sf::Vector2f base;
			// 最小缩放
			float min;
			// 最大缩放
			float max;
			// 速度(弧度/秒)
			float speed;

			// 动画总持续时间
			sf::Time duration;
			// 动画已持续时间
			sf::Time elapsed;
		};
	}

	// ============================================
	// 颜色效果
	// ============================================

	namespace color
	{
		// 线性变化
		class Linear final
		{
		public:
			// 动画开始时的颜色
			sf::Color from;
			// 动画结束时的颜色
			sf::Color to;

			// 动画总持续时间
			sf::Time duration;
			// 动画已持续时间
			sf::Time elapsed;
		};
	}
}
