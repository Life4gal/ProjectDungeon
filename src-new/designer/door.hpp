// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/door.hpp>

namespace pd::designer
{
	class Door final
	{
	public:
		using size_type = std::uint32_t;

		[[nodiscard]] static auto standard(size_type tile_x, size_type tile_y, blueprint::DoorDirection direction) noexcept -> blueprint::Door;
	};
}
