// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>
#include <optional>

#include <nlohmann/json_fwd.hpp>

namespace pd::config
{
	class ConfigReader final
	{
	public:
		using path_type = std::filesystem::path;

		using json_format = nlohmann::json;

		[[nodiscard]] static auto read_json(const std::filesystem::path& path) noexcept -> std::optional<json_format>;
	};
}
