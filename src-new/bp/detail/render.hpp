// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>
#include <vector>
#include <variant>

#include <bp/detail/transform.hpp>

namespace pd::bp
{
	// 渲染层(渲染顺序)
	enum class RenderLayer : std::uint32_t
	{
		// [0~7]

		FLOOR = 1uz << 0,
		WALL = 1uz << 1,
		DOOR = 1uz << 2,

		// [8~15]

		ENEMY = 1uz << 8,
		PLAYER = 1uz << 9,

		// [16~23]

		CLAW = 1uz << 16,
		PROJECTILE = 1 << 17,

		// [24~31]
		//
	};

	// 静态精灵
	class StaticSprite final
	{
	public:
		// 纹理路径
		std::string texture;

		// 纹理坐标
		Position position;

		// 纹理大小
		Size size;

		// 纹理原点(一般为size/2)
		Position origin;
	};

	// 动态精灵
	class DynamicSprite final
	{
	public:
		class Frame final
		{
		public:
			// 纹理路径
			std::string texture;

			// 纹理坐标
			Position position;
		};

		// 所有帧
		std::vector<Frame> frames;
		// 每一帧纹理大小
		Size size;
		// 每一帧纹理原点
		Position origin;

		// 每一帧持续时间(毫秒)
		int duration_ms;

		// 是否循环播放
		// 如果不循环则播放到最后一帧时不再从头播放
		bool looping;
		// 是否反向播放
		bool reversed;
	};

	class Sprite final : public std::variant<StaticSprite, DynamicSprite>
	{
	public:
		RenderLayer render_layer;
	};
}
