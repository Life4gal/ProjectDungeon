// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/dungeon.hpp>

namespace pd::component::dungeon
{
	// 所有关卡入口
	class Entries final
	{
	public:
		using entries_type = blueprint::Dungeon::entries_type;

		entries_type entries;
	};
}
