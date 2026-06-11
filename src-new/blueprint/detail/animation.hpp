// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <array>

#include <blueprint/detail/render.hpp>

namespace pd::blueprint
{
	// 基于方向的动画
	class Animation final
	{
	public:
		using size_type = std::uint32_t;

		class Group final
		{
		public:
			size_type begin_frame;
			size_type end_frame;
		};

		using groups_type = std::array<Group, 4>;

		// 渲染(必须是动态精灵)
		Sprite::Dynamic sprite_sheet;

		// 各个方向的动画索引
		// group[std::to_underlying(NORTH)] = {0, 3};
		// group[std::to_underlying(SOUTH)] = {4, 7};
		// group[std::to_underlying(NORTH)] = {8, 11};
		// group[std::to_underlying(NORTH)] = {12, 15};
		groups_type groups;
	};
}
