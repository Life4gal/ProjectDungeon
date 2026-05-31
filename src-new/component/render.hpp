// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <manager/resource_fwd.hpp>

#include <blueprint/def.hpp>

#include <component/render_effect.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

namespace pd::component::render
{
	// render_effect --> 渲染特效

	// 渲染层(渲染顺序)
	using blueprint::RenderLayer;

	// 生成时间(渲染层相同时优先渲染先生成的实体)
	class SpawnTime final
	{
	public:
		sf::Time time;
	};

	// 静态精灵
	namespace static_sprite
	{
		// 纹理的资源句柄
		class Texture final
		{
		public:
			manager::texture_handler texture;
		};

		// 纹理位置
		// 如果使用整张纹理渲染则为{0,0}
		class Position final
		{
		public:
			sf::Vector2f position;
		};

		// 纹理的大小
		// 如果使用整张纹理渲染则为整张纹理的大小
		class Size final
		{
		public:
			sf::Vector2f size;
		};

		// 纹理的原点(一般是TextureSize/2)
		class Origin final
		{
		public:
			sf::Vector2f origin;
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
				// static_sprite::Origin
				sf::Vector2f origin;
				// 持续时间
				sf::Time duration;
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

	// 其他渲染方式?
}
