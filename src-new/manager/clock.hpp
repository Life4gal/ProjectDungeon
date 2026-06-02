// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Time.hpp>

namespace pd::manager
{
	class Clock final
	{
	public:
		// 重置时间(一般不会调用)
		static auto restart() noexcept -> void;

		// 获取当前时间
		[[nodiscard]] static auto now() noexcept -> sf::Time;
	};
}
