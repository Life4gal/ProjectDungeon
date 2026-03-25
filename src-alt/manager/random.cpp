// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <manager/random.hpp>

namespace pd::manager
{
	auto Random::instance() noexcept -> Random&
	{
		static Random instance;
		return instance;
	}
}
