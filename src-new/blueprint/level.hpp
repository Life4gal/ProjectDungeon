// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <unordered_map>

#include <blueprint/room.hpp>

namespace pd::blueprint
{
	class RoomPosition final
	{
	public:
		using size_type = std::uint32_t;

		size_type x;
		size_type y;

		[[nodiscard]] constexpr auto operator==(const RoomPosition& other) const noexcept -> bool = default;
	};
}

template<>
struct std::hash<pd::blueprint::RoomPosition>
{
	[[nodiscard]] static auto operator()(const pd::blueprint::RoomPosition& position) noexcept -> std::size_t
	{
		return static_cast<std::size_t>(position.x) << 32 | static_cast<std::size_t>(position.y);
	}
};

namespace pd::blueprint
{
	class Level final
	{
	public:
		using rooms_type = std::unordered_map<RoomPosition, Room>;

		rooms_type rooms;
		RoomPosition start_position;
	};
}
