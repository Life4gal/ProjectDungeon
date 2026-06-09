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
		[[nodiscard]] static auto standard(blueprint::Direction direction) noexcept -> blueprint::Door;
	};
}
