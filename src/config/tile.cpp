// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/tile.hpp>

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace pd::config
{
	namespace
	{
		namespace from_json
		{
			auto load_tile_set(TileSet& tile_set, const ConfigReader::json_format& json) noexcept -> bool
			{
				const auto do_load = [&]<typename T>(
					const std::string_view name,
					const std::string_view key,
					T& dest,
					auto tile_maker
				) noexcept -> bool //
							requires requires
							{
								{ tile_maker(ConfigReader::json_format{}) } -> std::same_as<typename T::mapped_type>;
							}
				{
					const auto it = json.find(key);
					if (it == json.end())
					{
						SPDLOG_ERROR("瓦片集配置格式错误: 缺少'{}'字段!\n{}", key, json.dump(4));
						return false;
					}

					const auto& value = it.value();

					dest.reserve(value.size());
					for (const auto& [id, data]: value.items())
					{
						SPDLOG_INFO("正在加载{}瓦片[{}]...", name, id);

						try
						{
							auto tile = tile_maker(data);

							dest.emplace(id, std::move(tile));
						}
						catch (const nlohmann::json::exception& e)
						{
							SPDLOG_ERROR("解析{}瓦片配置时发生错误: {}\n{}", name, e.what(), data.dump(4));
						}
					}

					SPDLOG_INFO("加载{}配置完毕!", name);
					return true;
				};

				// wall_tiles
				if (not do_load(
					"墙壁",
					"walls",
					tile_set.wall_tiles,
					[](const ConfigReader::json_format& data) noexcept(false) -> WallTile
					{
						WallTile tile{};

						// Animation
						tile.animation_id = data.value("animation", std::string{});
						// Collision
						{
							const auto& collision = data["collision"];

							tile.collision_width = collision.value("width", 0);
							tile.collision_height = collision.value("height", 0);
							tile.enable_contact_event = false;
							tile.is_sensor = false;
						}

						return tile;
					}
				))
				{
					return false;
				}
				// floor_tiles
				if (not do_load(
					"地板",
					"floors",
					tile_set.floor_tiles,
					[](const ConfigReader::json_format& data) noexcept(false) -> FloorTile
					{
						FloorTile tile{};

						// Animation
						tile.animation_id = data.value("animation", std::string{});

						return tile;
					}
				))
				{
					return false;
				}
				// decoration_tiles
				if (not do_load(
					"装饰物",
					"decorations",
					tile_set.decoration_tiles,
					[](const ConfigReader::json_format& data) noexcept(false) -> DecorationTile
					{
						DecorationTile tile{};

						// Animation
						tile.animation_id = data.value("animation", std::string{});

						return tile;
					}
				))
				{
					return false;
				}
				// trigger_tiles
				if (not do_load(
					"触发器",
					"triggers",
					tile_set.trigger_tiles,
					[](const ConfigReader::json_format& data) noexcept(false) -> TriggerTile
					{
						TriggerTile tile{};

						// Animation
						tile.animation_id = data.value("animation", std::string{});
						// Collision
						{
							const auto& collision = data["collision"];
							tile.collision_width = collision.value("width", 0);
							tile.collision_height = collision.value("height", 0);
							tile.enable_contact_event = true;
							tile.is_sensor = collision.value("is_sensor", false);
						}

						return tile;
					}
				))
				{
					return false;
				}
				// key_tiles
				if (not do_load(
					"钥匙",
					"keys",
					tile_set.key_tiles,
					[](const ConfigReader::json_format& data) noexcept(false) -> KeyTile
					{
						KeyTile tile{};

						//Animation
						tile.animation_id = data.value("animation", std::string{});
						// Collision
						{
							const auto& collision = data["collision"];
							tile.collision_width = collision.value("width", 0);
							tile.collision_height = collision.value("height", 0);
							tile.enable_contact_event = true;
							// 传感器与否无所谓,反正接触之后就会销毁
							// 不过如果要修改应该对应地修改systems::update::process_contact_events
						}

						return tile;
					}
				))
				{
					return false;
				}
				// door_tiles
				if (not do_load(
					"门",
					"doors",
					tile_set.door_tiles,
					[](const ConfigReader::json_format& data) noexcept(false) -> DoorTile
					{
						DoorTile tile{};

						// Animation
						tile.animation_id = data.value("animation", std::string{});
						// Collision
						{
							const auto& collision = data["collision"];
							tile.collision_width = collision.value("width", 0);
							tile.collision_height = collision.value("height", 0);
							tile.enable_contact_event = true;
							// 默认为实体,因为门默认是关闭的,所以它应该阻止移动;当门打开时我们会切换成传感器
							tile.is_sensor = false;
						}

						return tile;
					}
				))
				{
					return false;
				}

				return true;
			}
		} // namespace from_json

		//
	}

	auto load_tile_set_from_json(TileSet& tile_set, const ConfigReader::json_format& json) noexcept -> bool
	{
		// "tiles": "/path/to/tiles.xxx"
		if (json.is_string())
		{
			const auto& config_path = json.get_ref<const std::string&>();
			SPDLOG_INFO("正在加载瓦片集指定配置文件[{}]...", config_path);

			// todo: 链接支持其他格式

			// "tiles": "/path/to/tiles.json"
			{
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("瓦片集指定的配置文件[{}]加载失败!", config_path);
					return false;
				}

				return from_json::load_tile_set(tile_set, *config);
			}
		}

		return from_json::load_tile_set(tile_set, json);
	}

	auto load_tile_set_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<TileSet>
	{
		TileSet tile_set{};
		if (not load_tile_set_from_json(tile_set, json))
		{
			return std::nullopt;
		}

		return tile_set;
	}
}
