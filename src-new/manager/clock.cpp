// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <manager/clock.hpp>

#include <SFML/System/Clock.hpp>

namespace pd::manager
{
	namespace
	{
		sf::Clock g_clock{};
	}

	auto Clock::restart() noexcept -> void
	{
		g_clock.restart();
	}

	auto Clock::now() noexcept -> sf::Time
	{
		return g_clock.getElapsedTime();
	}
}
