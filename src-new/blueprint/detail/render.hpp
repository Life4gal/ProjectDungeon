// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>
#include <vector>
#include <variant>

#include <blueprint/detail/transform.hpp>
#include <blueprint/detail/scalar.hpp>

#include <blueprint/def.hpp>

namespace pd::blueprint
{
	class Sprite final
	{
	public:
		// 静态精灵
		class Static final
		{
		public:
			// 纹理路径
			std::string texture;
			// 纹理坐标
			Position uv_position;
			// 纹理大小
			Size uv_size;

			// 锚点
			// render-position = entity-position - pivot
			Position pivot;
		};

		// 动态精灵
		// TODO: 一个实体一个动态精灵+根据行为更新下标 / 一个实体多个动态精灵+根据行为切换动画
		class Dynamic final
		{
		public:
			class Frame final
			{
			public:
				// 纹理路径
				std::string texture;
				// 纹理坐标
				Position uv_position;
				// 纹理大小
				Size uv_size;

				// 锚点
				// render-position = entity-position - pivot
				Position pivot;

				// 持续时间(毫秒)
				int duration_ms;
			};

			// 所有帧
			std::vector<Frame> frames;

			// 是否循环播放
			// 如果不循环则播放到最后一帧时不再从头播放
			bool looping;
			// 是否反向播放
			bool reversed;
			// 是否暂停
			bool pause;
		};

		using sprite_type = std::variant<Static, Dynamic>;

		sprite_type sprite;
		RenderLayer render_layer;
	};
}
