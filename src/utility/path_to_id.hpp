// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>

#include <prometheus/functional/functor.hpp>

#include <entt/core/hashed_string.hpp>

namespace pd::utility
{
	constexpr auto path_to_id = prometheus::functional::overloaded
	{
			[](const std::filesystem::path& path) noexcept -> entt::id_type
			{
				const auto& s = path.native();

				return entt::hashed_wstring{s.c_str(), s.size()};
			},
			[](const std::string_view path) noexcept -> entt::id_type
			{
				return entt::hashed_string{path.data(), path.size()};
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
