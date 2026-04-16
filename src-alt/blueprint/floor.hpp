// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>
#include <span>

#include <blueprint/def.hpp>

#include <game/constants.hpp>

#include <utility/fixed_matrix.hpp>

namespace pd::blueprint
{
	class Floor final
	{
	public:
		using layout_type = utility::FixedMatrix<RoomType, game::FloorHorizontalRoom, game::FloorVerticalRoom>;
		using size_type = layout_type::size_type;

		struct position_type
		{
			size_type x;
			size_type y;

			[[nodiscard]] constexpr auto operator==(const position_type& other) const noexcept -> bool
			{
				return x == other.x and y == other.y;
			}

			[[nodiscard]] constexpr auto operator<=>(const position_type& other) const noexcept -> auto
			{
				return std::tie(x, y) <=> std::tie(other.x, other.y);
			}
		};

		// 确定坐标是否在地图范围内
		[[nodiscard]] static auto is_within_bounds(position_type position) noexcept -> bool;

		// 生成楼层布局
		[[nodiscard]] static auto generate(position_type start_position, size_type count) noexcept -> layout_type;

		// 统计某个位置四周已放置(非NONE)房间的数量
		// 这也可以确定门的数量
		[[nodiscard]] static auto count_placed_neighbors(const layout_type& layout, position_type position) noexcept -> size_type;

		// 获取某个位置四周已放置(非NONE)房间的坐标
		[[nodiscard]] static auto get_placed_neighbors(const layout_type& layout, position_type position) noexcept -> std::vector<position_type>;

		// 获取某个位置四周未放置(NONE)房间的坐标
		[[nodiscard]] static auto get_unplaced_neighbors(const layout_type& layout, position_type position) noexcept -> std::vector<position_type>;

		// 获取所有指定位置四周未放置(NONE)房间的坐标(不含重复)
		[[nodiscard]] static auto get_unplaced_neighbors(const layout_type& layout, std::span<const position_type> positions) noexcept -> std::vector<position_type>;
	};
}
