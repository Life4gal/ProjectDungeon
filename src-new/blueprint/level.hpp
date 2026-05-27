// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <unordered_map>

#include <blueprint/room.hpp>

template<>
struct std::hash<pd::blueprint::LayoutPosition>
{
	static_assert(sizeof(std::size_t) >= 8);

	[[nodiscard]] static auto operator()(const pd::blueprint::LayoutPosition& position) noexcept -> std::size_t
	{
		return static_cast<std::size_t>(position.x) << 32 | static_cast<std::size_t>(position.y);
	}
};

namespace pd::blueprint
{
	[[nodiscard]] constexpr auto operator==(const LayoutPosition& lhs, const LayoutPosition& rhs) noexcept -> bool
	{
		return lhs.x == rhs.x and lhs.y == rhs.y;
	}

	// 关卡
	class Level final
	{
	public:
		std::unordered_map<LayoutPosition, Room> rooms;

		// 起始房间位置(避免遍历rooms查找RoomType::START)
		LayoutPosition start_position;
	};
}
