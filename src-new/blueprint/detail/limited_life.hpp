// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <variant>

namespace pd::blueprint
{
	// 有限生命
	class LimitedLife final
	{
	public:
		// 有限时间
		class Time final
		{
		public:
			// 最大存在时间(毫秒)
			int lifetime_ms;
		};

		using limited_life_type = std::variant<
			Time
		>;

		limited_life_type limited_life;
	};
}
