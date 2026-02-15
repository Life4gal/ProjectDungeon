// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>

#include <prometheus/functional/functor.hpp>

#include <entt/core/hashed_string.hpp>

namespace pd::utility
{
	constexpr auto string_to_id = prometheus::functional::overloaded
	{
			[](const std::string_view string) noexcept -> entt::id_type
			{
				return entt::hashed_string{string.data(), string.size()};
			},
			[](this auto& self, const std::string& string) noexcept -> entt::id_type
			{
				return self(std::string_view{string});
			},
			[](this auto& self, const char* string) noexcept -> entt::id_type
			{
				return self(std::string_view{string});
			}
	};
}
