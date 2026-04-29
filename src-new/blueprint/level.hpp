// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <unordered_map>

#include <blueprint/room.hpp>

namespace pd::blueprint
{
	// 房间类型
	enum class RoomType : std::uint8_t
	{
		// 起始房间
		START,
		// 标准房间
		STANDARD,
		// BOSS房间
		BOSS,
		// BOSS房间门钥匙房间
		KEY,
		// 商人房间
		MERCHANT,
		// 奖励房间
		BONUS,
		// 出口房间
		EXIT,
	};

	class Level final
	{
	public:
		// 房间类型可以重复(其实应该只有STANDARD房间会有重复)
		using rooms_type = std::unordered_multimap<RoomType, Room>;

		rooms_type rooms;
	};
}
