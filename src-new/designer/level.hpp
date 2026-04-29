// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/level.hpp>

namespace pd::designer
{
	class Level final
	{
	public:
		using size_type = std::uint32_t;

		// 关卡横向房间的最小数量
		constexpr static size_type horizontal_min_count = 6;
		// 关卡横向房间的最大数量
		constexpr static size_type horizontal_max_count = 13;
		// 关卡纵向房间的最小数量
		constexpr static size_type vertical_min_count = 3;
		// 关卡纵向房间的最大数量
		constexpr static size_type vertical_max_count = 7;
		// 关卡房间数量的基数(最小数量)
		constexpr static size_type base_count = 10;
		// 关卡房间数量的增长系数(count = base_count + level * factor)
		constexpr static size_type growth_factor = 4;
		// 关卡的最大房间数量
		constexpr static size_type max_count = base_count + growth_factor * 10;

		[[nodiscard]] static auto generate(size_type horizontal_count, size_type vertical_count, size_type count, size_type start_x, size_type start_y) noexcept -> blueprint::Level;
	};
}
