// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/dungeon.hpp>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace pd::config
{
	namespace
	{
		namespace from_json
		{
			auto load_dungeon_from_json(Dungeon& dungeon, const ConfigReader::json_format& json) noexcept -> bool
			{
				SPDLOG_INFO("============= 正在加载地下城... =============");

				try
				{
					dungeon.name = json["name"].get<std::string>();

					// level_set
					if (auto level_set = load_level_set_from_json(json);
						not level_set.has_value())
					{
						SPDLOG_ERROR("地下城[{}]的关卡集加载失败!", dungeon.name);
						return false;
					}
					else
					{
						dungeon.level_set = *std::move(level_set);
					}

					// level_order
					{
						const auto& level_order = json["level_order"];
						dungeon.level_order.reserve(level_order.size());

						for (const auto& id: level_order)
						{
							auto s = id.get<std::string>();

							if (const auto it = dungeon.level_set.find(s);
								it == dungeon.level_set.end())
							{
								SPDLOG_ERROR("关卡[{}]不存在!", s);
								return false;
							}

							dungeon.level_order.emplace_back(std::move(s));
						}
					}

					return true;
				}
				catch (const nlohmann::json::exception& e)
				{
					SPDLOG_ERROR("解析地下城配置时发生错误:{}", e.what());
					return false;
				}
			}
		}

		//
	}

	auto load_dungeon_from_json(Dungeon& dungeon, const ConfigReader::json_format& json) noexcept -> bool
	{
		return from_json::load_dungeon_from_json(dungeon, json);
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
}
