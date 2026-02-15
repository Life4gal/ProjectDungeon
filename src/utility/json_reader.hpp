// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>
#include <optional>

#include <nlohmann/json.hpp>

namespace pd::utility
{
	// JSON配置文件读取器
	class JsonReader final
	{
	public:
		using json_type = nlohmann::json;
		using path_type = std::filesystem::path;

		// 从文件读取JSON
		// 如果成功则返回JSON对象,失败返回std::nullopt
		[[nodiscard]] static auto read_file(const path_type& path) noexcept -> std::optional<json_type>;
	};
}
