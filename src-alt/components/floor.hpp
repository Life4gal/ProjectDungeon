// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/floor.hpp>

namespace pd::components::floor
{
	// ==========================================
	// 上下文组件
	// ==========================================

	// 楼层布局
	class Layout final
	{
	public:
		using layout_type = blueprint::Floor::layout_type;

		layout_type layout;
	};

	// 当前生成房间数量
	class RoomCount final
	{
	public:
		std::uint32_t count;
	};

	// 当前所在房间坐标
	class CurrentRoom final
	{
	public:
		std::uint32_t x;
		std::uint32_t y;
	};

	// ==========================================
	// 实体组件
	// ==========================================

	//
}
