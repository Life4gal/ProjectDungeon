// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/player.hpp>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace pd::config
{
	namespace from_json
	{
		// 支持其他格式后,这些接口其他文件也许用得到?先不要放到匿名命名空间了
		extern auto load_player(Player& player, const ConfigReader::json_format& json) noexcept -> bool;
		extern auto load_player_set(PlayerSet& player_set, const ConfigReader::json_format& json) noexcept -> bool;

		[[nodiscard]] auto load_player(Player& player, const ConfigReader::json_format& json) noexcept -> bool
		{
			try
			{
				player.animation_id = json["animation_id"].get<std::string>();

				player.health = json["health"].get<float>();
				player.mana = json["mana"].get<float>();
				player.speed = json["speed"].get<float>();

				const auto weapons_it = json.find("weapons");
				if (weapons_it == json.end())
				{
					SPDLOG_ERROR("玩家配置格式错误: 缺少'weapons'字段!\n{}", json.dump(4));
					return false;
				}

				const auto& weapons = weapons_it.value();
				if (weapons.empty())
				{
					SPDLOG_ERROR("玩家配置格式错误: 'weapons'字段内容为空!\n{}", json.dump(4));
					return false;
				}

				player.weapons.reserve(weapons.size());
				for (const auto& data: weapons)
				{
					auto item_name = data.get<std::string>();

					player.weapons.emplace_back(std::move(item_name));
				}

				return true;
			}
			catch (const nlohmann::json::exception& e)
			{
				SPDLOG_ERROR("解析玩家配置时发生错误: {}\n{}", e.what(), json.dump(4));
				return false;
			}
		}

		[[nodiscard]] auto load_player_set(PlayerSet& player_set, const ConfigReader::json_format& json) noexcept -> bool
		{
			player_set.reserve(json.size());

			for (const auto& [id, data]: json.items())
			{
				SPDLOG_INFO("正在加载玩家[{}]数据...", id);

				Player player{};
				// 调用完整接口,如此即使后续支持格式有变,此处也无需改动
				if (not load_player_from_json(player, data))
				{
					SPDLOG_ERROR("加载玩家[{}]失败!", id);
					continue;
				}

				player_set.emplace(id, std::move(player));

				SPDLOG_INFO("加载玩家[{}]完毕!", id);
			}

			return true;
		}
	} // namespace from_json

	auto load_player_from_json(Player& player, const ConfigReader::json_format& json) noexcept -> bool
	{
		// "player-id": "/path/to/player-config.xxx"
		if (json.is_string())
		{
			const auto& config_path = json.get_ref<const std::string&>();
			SPDLOG_INFO("正在加载玩家指定配置文件[{}]...", config_path);

			// todo: 链接支持其他格式

			// "player-id": "/path/to/player-config.json"
			{
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("玩家指定的配置文件[{}]加载失败!", config_path);
					return false;
				}

				return from_json::load_player(player, *config);
			}
		}

		return from_json::load_player(player, json);
	}

	auto load_player_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<Player>
	{
		Player player{};
		if (not load_player_from_json(player, json))
		{
			return std::nullopt;
		}

		return player;
	}

	auto load_player_set_from_json(PlayerSet& player_set, const ConfigReader::json_format& json) noexcept -> bool
	{
		// "players": "/path/to/players.xxx"
		if (json.is_string())
		{
			const auto& config_path = json.get_ref<const std::string&>();
			SPDLOG_INFO("正在加载玩家集指定配置文件[{}]...", config_path);

			// todo: 链接支持其他格式

			// "players": "/path/to/players.json"
			{
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("玩家集指定的配置文件[{}]加载失败!", config_path);
					return false;
				}

				return from_json::load_player_set(player_set, *config);
			}
		}

		return from_json::load_player_set(player_set, json);
	}

	auto load_player_set_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<PlayerSet>
	{
		PlayerSet player_set{};
		if (not load_player_set_from_json(player_set, json))
		{
			return std::nullopt;
		}

		return player_set;
	}
} // namespace pd::config
