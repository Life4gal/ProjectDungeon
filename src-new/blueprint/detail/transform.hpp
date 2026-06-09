// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <bit>

namespace pd::blueprint
{
	// 世界坐标
	class Position final
	{
	public:
		float x;
		float y;
	};

	// 缩放
	class Scale final
	{
	public:
		float x;
		float y;
	};

	// 旋转
	class Rotation final
	{
	public:
		// degree
		float rotation;
	};

	[[nodiscard]] constexpr auto operator==(const Position& lhs, const Position& rhs) noexcept -> bool
	{
		return lhs.x == rhs.x and lhs.y == rhs.y;
	}
}

template<>
struct std::hash<pd::blueprint::Position>
{
	static_assert(sizeof(std::size_t) >= 8);

	[[nodiscard]] static auto operator()(const pd::blueprint::Position& position) noexcept -> std::size_t
	{
		const auto x = std::bit_cast<std::uint32_t>(position.x);
		const auto y = std::bit_cast<std::uint32_t>(position.y);

		return static_cast<std::size_t>(x) << 32 | static_cast<std::size_t>(y);
	}
};
