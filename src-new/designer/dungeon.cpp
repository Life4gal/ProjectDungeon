// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/dungeon.hpp>

namespace pd::designer
{
	auto Dungeon::standard() noexcept -> blueprint::Dungeon
	{
		using entry_type = blueprint::Dungeon::Entry;

		// level 1
		constexpr entry_type entry1
		{
				.seed = 123456,
				.horizontal_count = 8,
				.vertical_count = 5,
				.count = 15,
				.start_x = 4,
				.start_y = 2,
		};
		// level 2
		constexpr entry_type entry2
		{
				.seed = 123456,
				.horizontal_count = 10,
				.vertical_count = 6,
				.count = 20,
				.start_x = 5,
				.start_y = 3,
		};
		// level 3
		constexpr entry_type entry3
		{
				.seed = 123456,
				.horizontal_count = 10,
				.vertical_count = 8,
				.count = 25,
				.start_x = 5,
				.start_y = 4,
		};

		return
		{
				.entries = {entry1, entry2, entry3},
		};
	}
}
