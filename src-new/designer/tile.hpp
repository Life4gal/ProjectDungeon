// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/tile.hpp>

namespace pd::designer
{
	class Tile final
	{
	public:
		// 地板
		[[nodiscard]] static auto floor() noexcept -> blueprint::Tile;

		// 墙壁
		[[nodiscard]] static auto wall() noexcept -> blueprint::Tile;
	};
}
