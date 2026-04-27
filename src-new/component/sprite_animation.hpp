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
	// sprite

	// 所有精灵
	class Sprites final
	{
	public:
		// 切换精灵时读取的信息
		class Sprite final
		{
		public:
			// sprite::Texture
			manager::texture_handler texture;
			// sprite::Position
			sf::Vector2i position;
			// sprite::Size
			sf::Vector2i size;
			// sprite::Origin
			sf::Vector2i origin;

			// blueprint::SpriteAnimation::durations_ms
			int duration_ms;
		};

		std::vector<Sprite> sprites;
	};

	// 精灵总数(sprites.size())
	class Total final
	{
	public:
		std::size_t total;
	};

	// 当前动画帧计时器
	class Timer final
	{
	public:
		// 当前帧总持续时间
		sf::Time duration;
		// 当前帧已持续时间
		sf::Time elapsed;
	};

	// 当前动画帧索引
	class Index final
	{
	public:
		std::size_t index;
	};

	// 标记当前动画是否循环
	// 如果不循环则不存在该数据
	class Looping final {};

	// 标记当前动画是否反向播放
	// 如果不反向播放则不存在该数据
	class Reversed final {};

	// 标记当前动画是否暂停
	// 如果不暂停则不存在该数据
	class Paused final {};

	// 标记当前动画已经结束(此数据可以避免那些非循环动画也参与更新)
	// 如果未结束则不存在该数据
	class Ended final {};
}
