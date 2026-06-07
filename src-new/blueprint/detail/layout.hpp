// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <utility>

namespace pd::blueprint
{
	// 布局坐标
	class LayoutPosition final
	{
	public:
		using size_type = std::uint32_t;

		size_type x;
		size_type y;
	};

	[[nodiscard]] constexpr auto operator==(const LayoutPosition& lhs, const LayoutPosition& rhs) noexcept -> bool
	{
		return lhs.x == rhs.x and lhs.y == rhs.y;
	}
}

template<>
struct std::hash<pd::blueprint::LayoutPosition>
{
	static_assert(sizeof(std::size_t) >= 8);

	[[nodiscard]] static auto operator()(const pd::blueprint::LayoutPosition& position) noexcept -> std::size_t
	{
		return static_cast<std::size_t>(position.x) << 32 | static_cast<std::size_t>(position.y);
	}
};
