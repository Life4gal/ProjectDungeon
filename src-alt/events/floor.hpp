// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::events::floor
{
	// 创建楼层
	class GenerateRequest final
	{
	public:
		// 房间数量
		std::uint32_t count;
		// 起始房间坐标X
		std::uint32_t start_x;
		// 起始房间坐标Y
		std::uint32_t start_y;
	};

	// 进入房间
	class Entered final
	{
	public:
		std::uint32_t x;
		std::uint32_t y;
	};

	// 清理房间
	class Cleared final
	{
	public:
		std::uint32_t x;
		std::uint32_t y;
	};

	// 离开房间
	class Left final
	{
	public:
		std::uint32_t x;
		std::uint32_t y;
	};
}
