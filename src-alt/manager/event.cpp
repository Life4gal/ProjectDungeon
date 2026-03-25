// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <manager/event.hpp>

namespace pd::manager
{
	auto Event::instance() noexcept -> Event&
	{
		static Event instance;
		return instance;
	}
}
