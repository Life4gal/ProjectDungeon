// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <type_traits>

namespace pd::config
{
	// 位置
	class Position final
	{
	public:
		int x;
		int y;

		[[nodiscard]] constexpr auto operator<=>(const Position&) const noexcept = default;
	};
}

template<>
struct std::hash<pd::config::Position>
{
	static auto operator()(const pd::config::Position& position) noexcept -> std::size_t
	{
		return static_cast<std::size_t>(position.x) << 32 | static_cast<std::size_t>(position.y);
	}
};
