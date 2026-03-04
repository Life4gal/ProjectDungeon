// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/dungeon.hpp>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace pd::config
{
	namespace from_json
	{
		// 支持其他格式后,这些接口其他文件也许用得到?先不要放到匿名命名空间了
		extern auto load_dungeon(Dungeon& dungeon, const ConfigReader::json_format& json) noexcept -> bool;

		[[nodiscard]] auto load_dungeon(Dungeon& dungeon, const ConfigReader::json_format& json) noexcept -> bool
		{
			try
			{
				// name
				{
					const auto name_it = json.find("name");
					if (name_it == json.end())
					{
						SPDLOG_ERROR("地下城配置格式错误: 缺少'name'字段!\n{}", json.dump(4));
						return false;
					}

					const auto& name = name_it.value();
					dungeon.name = name.get<std::string>();
					SPDLOG_INFO("读取到地下城名称: [{}]", dungeon.name);
				}

				// level_set
				{
					const auto levels_it = json.find("levels");
					if (levels_it == json.end())
					{
						SPDLOG_ERROR("地下城配置格式错误: 缺少'levels'字段!\n{}", json.dump(4));
						return false;
					}

					const auto& levels = levels_it.value();
					if (auto level_set = load_level_set_from_json(levels);
						not level_set.has_value())
					{
						SPDLOG_ERROR("无法加载关卡集!地下城加载失败!");
						return false;
					}
					else
					{
						dungeon.level_set = *std::move(level_set);
					}
				}

				// level_order
				{
					const auto level_order_it = json.find("level_order");
					if (level_order_it == json.end())
					{
						SPDLOG_ERROR("地下城配置格式错误: 缺少'level_order'字段!\n{}", json.dump(4));
						return false;
					}

					const auto& level_order = level_order_it.value();
					dungeon.level_order.reserve(level_order.size());

					for (const auto& data: level_order)
					{
						if (not data.is_string())
						{
							SPDLOG_ERROR("指定的关卡ID格式错误!\n{}", data.dump(4));
							return false;
						}

						auto id = data.get<std::string>();
						if (const auto it = dungeon.level_set.find(id);
							it == dungeon.level_set.end())
						{
							SPDLOG_ERROR("指定的关卡[{}]不存在!", id);
							return false;
						}

						dungeon.level_order.emplace_back(std::move(id));
					}
				}

				return true;
			}
			catch (const nlohmann::json::exception& e)
			{
				SPDLOG_ERROR("解析地下城配置时发生错误: {}\n{}", e.what(), json.dump(4));
				return false;
			}
		}
	} // namespace from_json

	auto load_dungeon_from_json(Dungeon& dungeon, const ConfigReader::json_format& json) noexcept -> bool
	{
		return from_json::load_dungeon(dungeon, json);
	}

	auto load_dungeon_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<Dungeon>
	{
		Dungeon dungeon{};
		if (not load_dungeon_from_json(dungeon, json))
		{
			return std::nullopt;
		}

		return dungeon;
	}
} // namespace pd::config
