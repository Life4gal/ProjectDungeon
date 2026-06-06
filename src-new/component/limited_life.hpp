// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Time.hpp>

namespace pd::component::limited_life
{
	// =================
	// 时间
	// =================

	namespace time
	{
		// 总时长
		class Lifetime final
		{
		public:
			sf::Time lifetime;
		};

		// 已历时
		class Elapsed final
		{
		public:
			sf::Time elapsed;
		};
	}

	// =================
	// 
	// =================
}
