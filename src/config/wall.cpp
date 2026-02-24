// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/wall.hpp>

#include <utility/json_reader.hpp>

#include <spdlog/spdlog.h>

namespace pd::config
{
	auto load_wall(const std::filesystem::path& path) noexcept -> WallSet
	{
		WallSet wall_set{};

		const auto json_opt = utility::JsonReader::read_file(path);
		if (not json_opt.has_value())
		{
			return wall_set;
		}

		const auto& json = json_opt.value();

		try
		{
			if (not json.contains("walls"))
			{
				SPDLOG_ERROR("墙壁配置格式错误:缺少'walls'字段!");
				return wall_set;
			}

			for (const auto& walls = json["walls"];
			     const auto& [wall_name, wall_data]: walls.items())
			{
				Wall wall{};

				wall.texture_path = wall_data["texture_path"].get<std::string>();
				wall.texture_x = wall_data["texture_x"].get<int>();
				wall.texture_y = wall_data["texture_y"].get<int>();
				wall.texture_width = wall_data["texture_width"].get<int>();
				wall.texture_height = wall_data["texture_height"].get<int>();

				wall.collision_width = wall_data.value("collision_width", static_cast<float>(wall.texture_width));
				wall.collision_height = wall_data.value("collision_height", static_cast<float>(wall.texture_height));

				wall.health = wall_data.value("health", -1);

				SPDLOG_DEBUG("加载墙壁[{}]:{}x{}", wall_name, wall.texture_width, wall.texture_height);
				wall_set.emplace(wall_name, std::move(wall));
			}

			SPDLOG_INFO("加载墙壁配置成功!共有[{}]种墙壁", wall_set.size());
		}
		catch (const nlohmann::json::exception& e)
		{
			SPDLOG_ERROR("解析墙壁配置时发生错误:{}", e.what());
		}

		return wall_set;
	}
} // namespace pd::config
