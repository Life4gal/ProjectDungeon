// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <manager/random.hpp>

#include <blueprint/level.hpp>

namespace pd::blueprint
{
	// 地下城
	class Dungeon final
	{
	public:
		using size_type = Level::size_type;

		// 关卡入口
		class Entry final
		{
		public:
			using seed_type = manager::Random::result_type;

			// 关卡随机数种子
			seed_type seed;

			// 关卡横向房间的最大数量
			size_type horizontal_count;
			// 关卡纵向房间的最大数量
			size_type vertical_count;
			// 关卡房间数量
			size_type count;
			// 关卡入口横向坐标
			size_type start_x;
			// 关卡入口纵向坐标
			size_type start_y;
		};

		using entries_type = std::vector<Entry>;

		// 所有关卡入口
		entries_type entries;
	};
}
