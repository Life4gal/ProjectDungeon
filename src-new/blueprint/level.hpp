// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <unordered_map>

#include <blueprint/room.hpp>

namespace pd::blueprint
{
	// 关卡
	class Level final
	{
	public:
		using position_type = LayoutPosition;
		using size_type = LayoutPosition::size_type;

		std::unordered_map<position_type, Room> rooms;

		// 起始房间位置(避免遍历rooms查找RoomType::START)
		position_type start_position;
	};
}
