// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string_view>

namespace pd::manager
{
	class Internationalization final
	{
	public:
		// TODO: 载入数据

		[[nodiscard]] static auto map(std::string_view key) noexcept -> std::string_view;
	};
}
