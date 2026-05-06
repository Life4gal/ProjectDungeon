// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <component/sprite.hpp>

#include <SFML/System/Time.hpp>

namespace pd::component::sprite_animation
{
	// 精灵动画 == 精灵 + 每帧切换
	// sprite --> 精灵动画精灵

	// 精灵动画所有帧信息
	class Frames final
	{
	public:
		class Frame final
		{
		public:
			// sprite::Texture
			manager::texture_handler texture;
			// sprite::TexturePosition
			sf::Vector2f position;
		};

		std::vector<Frame> frames;
	};

	// 总帧数
	class FramesCount final
	{
	public:
		// Frames::frames.size()
		std::size_t frames_count;
	};

	// 每一帧的持续时间
	class Duration final
	{
	public:
		sf::Time duration;
	};

	// 帧计时器
	class Timer final
	{
	public:
		sf::Time elapsed;
	};

	// 帧索引
	class Index final
	{
	public:
		std::size_t index;
	};

	// 当前动画播放模式
	enum class Mode : std::uint8_t
	{
		// 单次播放
		ONE_SHOT,
		// 循环播放
		LOOP,
	};

	// 当前动画播放方向
	enum class Direction : std::uint8_t
	{
		// 正向播放
		FORWARD,
		// 反向播放
		BACKWARD,
	};

	// 标记当前动画是否暂停
	// 如果不暂停则不存在该数据
	class Paused final {};

	// 标记当前动画已经结束(此数据可以避免那些非循环动画也参与更新)
	// 如果未结束则不存在该数据
	class Ended final {};
}
