// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <utility/json_reader.hpp>

#include <fstream>

#include <spdlog/spdlog.h>

namespace pd::utility
{
	auto JsonReader::read_file(const path_type& path) noexcept -> std::optional<json_type>
	{
		const auto s = path.string();

		if (not exists(path))
		{
			SPDLOG_ERROR("读取JSON[{:<20}]失败!文件不存在!", s);
			return std::nullopt;
		}

		try
		{
			std::ifstream file{path};
			if (not file.is_open())
			{
				SPDLOG_ERROR("读取JSON[{:<20}]失败!无法打开文件!", s);
				return std::nullopt;
			}

			json_type json;
			file >> json;

			SPDLOG_INFO("读取JSON[{:<20}]成功!", s);
			return json;
		}
		catch (const nlohmann::json::exception& e)
		{
			SPDLOG_ERROR("解析JSON[{:<20}]失败!错误: {}", s, e.what());
			return std::nullopt;
		}
		catch (const std::exception& e)
		{
			SPDLOG_ERROR("读取JSON[{:<20}]失败!错误: {}", s, e.what());
			return std::nullopt;
		}
	}
}
