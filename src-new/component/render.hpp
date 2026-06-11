// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <manager/resource_fwd.hpp>

#include <blueprint/def.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

namespace pd::component::render
{
	// 渲染层(渲染顺序)
	using blueprint::RenderLayer;

	// 静态精灵
	namespace static_sprite
	{
		// 纹理的资源句柄
		class Texture final
		{
		public:
			manager::texture_handler texture;
		};

		// 纹理的坐标
		// 如果使用整张纹理渲染则为{0,0}
		class UvPosition final
		{
		public:
			sf::Vector2f position;
		};

		// 纹理的大小
		// 如果使用整张纹理渲染则为整张纹理的大小
		class UvSize final
		{
		public:
			sf::Vector2f size;
		};

		// 锚点
		class Pivot final
		{
		public:
			sf::Vector2f pivot;
		};
	}

	// 动态精灵
	namespace dynamic_sprite
	{
		// 动态精灵 = 静态精灵 + 切换纹理

		// 精灵动画所有帧信息
		class Frames final
		{
		public:
			class Frame final
			{
			public:
				// static_sprite::Texture
				manager::texture_handler texture;
				// static_sprite::Position
				sf::Vector2f position;
				// static_sprite::Size
				sf::Vector2f size;
				// static_sprite::Pivot
				sf::Vector2f pivot;
				// 持续时间
				sf::Time duration;
			};

			std::vector<Frame> frames;
		};

		// 起始帧索引
		// 一般为0,不过可以修改该值来控制渲染的帧
		class BeginFrameIndex final
		{
		public:
			std::size_t index;
		};

		// 结束帧索引
		// 一般为Frames::frames.size(),不过可以修改该值来控制渲染的帧
		class EndFrameIndex final
		{
		public:
			std::size_t index;
		};

		// 当前渲染的帧
		class CurrentFrameIndex final
		{
		public:
			std::size_t index;
		};

		// 当前渲染的帧已持续时间
		class CurrentFrameElapsed final
		{
		public:
			sf::Time elapsed;
		};

		// 当前动画播放模式
		using blueprint::AnimationMode;

		// 当前动画播放方向
		using blueprint::AnimationDirection;

		// 标记当前动画是否暂停
		// 如果不暂停则不存在该数据
		class Paused final {};

		// 标记当前动画已经结束(此数据可以避免那些非循环动画也参与更新)
		// 如果未结束则不存在该数据
		class Ended final {};
	}

	// 其他渲染方式?
}
