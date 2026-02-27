// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>

namespace pd::utility
{
	class StringHash final
	{
	public:
		using is_transparent = int;

		[[nodiscard]] static auto operator()(const std::string& string) noexcept -> std::size_t
		{
			return std::hash<std::string>{}(string);
		}

		[[nodiscard]] static auto operator()(const std::string_view string) noexcept -> std::size_t
		{
			return std::hash<std::string_view>{}(string);
		}
	};
}
