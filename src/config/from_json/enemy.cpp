// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/enemy.hpp>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace pd::config
{
	namespace from_json
	{
		// 支持其他格式后,这些接口其他文件也许用得到?先不要放到匿名命名空间了
		extern auto load_enemy(Enemy& enemy, const ConfigReader::json_format& json) noexcept -> bool;
		extern auto load_enemy_set(EnemySet& enemy_set, const ConfigReader::json_format& json) noexcept -> bool;

		[[nodiscard]] auto load_enemy(Enemy& enemy, const ConfigReader::json_format& json) noexcept -> bool
		{
			try
			{
				enemy.animation_id = json["animation_id"].get<std::string>();

				enemy.health = json["health"].get<float>();
				enemy.mana = json["mana"].get<float>();
				enemy.speed = json["speed"].get<float>();
				enemy.collision_damage = json["collision_damage"].get<float>();

				const auto weapons_it = json.find("weapons");
				if (weapons_it == json.end())
				{
					SPDLOG_ERROR("敌人配置格式错误: 缺少'weapons'字段!\n{}", json.dump(4));
					return false;
				}

				const auto& weapons = weapons_it.value();
				if (weapons.empty())
				{
					SPDLOG_ERROR("敌人配置格式错误: 'weapons'字段内容为空!\n{}", json.dump(4));
					return false;
				}

				enemy.weapons.reserve(weapons.size());
				for (const auto& data: weapons)
				{
					auto item_name = data.get<std::string>();

					enemy.weapons.emplace_back(std::move(item_name));
				}

				return true;
			}
			catch (const nlohmann::json::exception& e)
			{
				SPDLOG_ERROR("解析敌人配置时发生错误: {}\n{}", e.what(), json.dump(4));
				return false;
			}
		}

		[[nodiscard]] auto load_enemy_set(EnemySet& enemy_set, const ConfigReader::json_format& json) noexcept -> bool
		{
			enemy_set.reserve(json.size());

			for (const auto& [id, data]: json.items())
			{
				SPDLOG_INFO("正在加载敌人[{}]数据...", id);

				Enemy enemy{};
				// 调用完整接口,如此即使后续支持格式有变,此处也无需改动
				if (not load_enemy_from_json(enemy, data))
				{
					SPDLOG_ERROR("加载敌人[{}]失败!", id);
					continue;
				}

				enemy_set.emplace(id, std::move(enemy));

				SPDLOG_INFO("加载敌人[{}]完毕!", id);
			}

			return true;
		}
	} // namespace from_json

	auto load_enemy_from_json(Enemy& enemy, const ConfigReader::json_format& json) noexcept -> bool
	{
		// "enemy-id": "/path/to/enemy-config.xxx"
		if (json.is_string())
		{
			const auto& config_path = json.get_ref<const std::string&>();
			SPDLOG_INFO("正在加载敌人指定配置文件[{}]...", config_path);

			// todo: 链接支持其他格式

			// "enemy-id": "/path/to/enemy-config.json"
			{
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("敌人指定的配置文件[{}]加载失败!", config_path);
					return false;
				}

				return from_json::load_enemy(enemy, *config);
			}
		}

		return from_json::load_enemy(enemy, json);
	}

	auto load_enemy_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<Enemy>
	{
		Enemy enemy{};
		if (not load_enemy_from_json(enemy, json))
		{
			return std::nullopt;
		}

		return enemy;
	}

	auto load_enemy_set_from_json(EnemySet& enemy_set, const ConfigReader::json_format& json) noexcept -> bool
	{
		// "enemies": "/path/to/enemies.xxx"
		if (json.is_string())
		{
			const auto& config_path = json.get_ref<const std::string&>();
			SPDLOG_INFO("正在加载敌人集指定配置文件[{}]...", config_path);

			// todo: 链接支持其他格式

			// "enemies": "/path/to/enemies.json"
			{
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("敌人集指定的配置文件[{}]加载失败!", config_path);
					return false;
				}

				return from_json::load_enemy_set(enemy_set, *config);
			}
		}

		return from_json::load_enemy_set(enemy_set, json);
	}

	auto load_enemy_set_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<EnemySet>
	{
		EnemySet enemy_set{};
		if (not load_enemy_set_from_json(enemy_set, json))
		{
			return std::nullopt;
		}

		return enemy_set;
	}
} // namespace pd::config
