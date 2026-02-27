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
			auto load_wall_set(const ConfigReader::json_format& json) noexcept -> std::optional<WallTileSet>
			{
				const auto walls_it = json.find("walls");
				if (walls_it == json.end())
				{
					SPDLOG_ERROR("瓦片集配置格式错误: 缺少'walls'字段!\n{}", json.dump(4));
					return std::nullopt;
				}

				const auto& walls = walls_it.value();

				WallTileSet wall_set{};
				wall_set.reserve(walls.size());

				for (const auto& [id, wall]: walls.items())
				{
					SPDLOG_INFO("加载墙壁瓦片[{}]...", id);

					try
					{
						WallTile tile{};

						// Animation
						tile.animation_id = wall.value("animation", std::string{});
						// Collision
						{
							const auto& collision = wall["collision"];

							tile.collision_width = collision.value("width", 0);
							tile.collision_height = collision.value("height", 0);
							tile.enable_contact_event = false;
							tile.is_sensor = false;
						}

						wall_set.emplace(id, std::move(tile));
					}
					catch (const nlohmann::json::exception& e)
					{
						SPDLOG_ERROR("解析WallTile配置失败: {}\n{}", e.what(), wall.dump(4));
					}

					SPDLOG_INFO("加载墙壁瓦片[{}]完毕!", id);
				}

				return wall_set;
			}

			auto load_floor_set(const ConfigReader::json_format& json) noexcept -> std::optional<FloorTileSet>
			{
				const auto floors_it = json.find("floors");
				if (floors_it == json.end())
				{
					SPDLOG_ERROR("瓦片集配置格式错误: 缺少'floors'字段!\n{}", json.dump(4));
					return std::nullopt;
				}

				const auto& floors = floors_it.value();

				FloorTileSet floor_set{};
				floor_set.reserve(floors.size());

				for (const auto& [id, floor]: floors.items())
				{
					SPDLOG_INFO("加载地板瓦片[{}]...", id);

					try
					{
						FloorTile tile{};

						// Animation
						tile.animation_id = floor.value("animation", std::string{});

						floor_set.emplace(id, std::move(tile));
					}
					catch (const nlohmann::json::exception& e)
					{
						SPDLOG_ERROR("解析FloorTile配置失败: {}\n{}", e.what(), floor.dump(4));
					}

					SPDLOG_INFO("加载地板瓦片[{}]完毕!", id);
				}

				return floor_set;
			}

			auto load_decoration_set(const ConfigReader::json_format& json) noexcept -> std::optional<DecorationTileSet>
			{
				const auto decorations_it = json.find("decorations");
				if (decorations_it == json.end())
				{
					SPDLOG_ERROR("瓦片集配置格式错误: 缺少'decorations'字段!\n{}", json.dump(4));
					return std::nullopt;
				}

				const auto& decorations = decorations_it.value();

				DecorationTileSet decoration_set{};
				decoration_set.reserve(decorations.size());

				for (const auto& [id, decoration]: decorations.items())
				{
					SPDLOG_INFO("加载地板瓦片[{}]...", id);

					try
					{
						DecorationTile tile{};

						// Animation
						tile.animation_id = decoration.value("animation", std::string{});

						decoration_set.emplace(id, std::move(tile));
					}
					catch (const nlohmann::json::exception& e)
					{
						SPDLOG_ERROR("解析DecorationTile配置失败: {}\n{}", e.what(), decoration.dump(4));
					}

					SPDLOG_INFO("加载地板瓦片[{}]完毕!", id);
				}
				return decoration_set;
			}

			auto load_trigger_set(const ConfigReader::json_format& json) noexcept -> std::optional<TriggerTileSet>
			{
				const auto triggers_it = json.find("triggers");
				if (triggers_it == json.end())
				{
					SPDLOG_ERROR("瓦片集配置格式错误: 缺少'triggers'字段!\n{}", json.dump(4));
					return std::nullopt;
				}

				const auto& triggers = triggers_it.value();

				TriggerTileSet trigger_set{};
				trigger_set.reserve(triggers.size());

				for (const auto& [id, trigger]: triggers.items())
				{
					SPDLOG_INFO("加载触发器瓦片[{}]...", id);

					try
					{
						TriggerTile tile{};

						// Animation
						tile.animation_id = trigger.value("animation", std::string{});
						// Collision
						{
							const auto& collision = trigger["collision"];

							tile.collision_width = collision.value("width", 0);
							tile.collision_height = collision.value("height", 0);
							tile.enable_contact_event = true;
							tile.is_sensor = collision.value("is_sensor", false);
						}

						trigger_set.emplace(id, std::move(tile));
					}
					catch (const nlohmann::json::exception& e)
					{
						SPDLOG_ERROR("解析TriggerTile配置失败: {}\n{}", e.what(), trigger.dump(4));
					}

					SPDLOG_INFO("加载触发器瓦片[{}]完毕!", id);
				}
				return trigger_set;
			}

			auto load_door_set(const ConfigReader::json_format& json) noexcept -> std::optional<DoorTileSet>
			{
				const auto doors_it = json.find("doors");
				if (doors_it == json.end())
				{
					SPDLOG_ERROR("瓦片集配置格式错误: 缺少'doors'字段!\n{}", json.dump(4));
					return std::nullopt;
				}

				const auto& doors = doors_it.value();

				DoorTileSet door_set{};
				door_set.reserve(doors.size());

				for (const auto& [id, door]: doors.items())
				{
					SPDLOG_INFO("加载门瓦片[{}]...", id);

					try
					{
						DoorTile tile{};

						// Animation
						tile.animation_id = door.value("animation", std::string{});
						// Collision
						{
							const auto& collision = door["collision"];

							tile.collision_width = collision.value("width", 0);
							tile.collision_height = collision.value("height", 0);
							tile.enable_contact_event = true;
							// 默认为实体,因为门默认是关闭的,所以它应该阻止移动;当门打开时我们会切换成传感器
							tile.is_sensor = false;
						}

						door_set.emplace(id, std::move(tile));
					}
					catch (const nlohmann::json::exception& e)
					{
						SPDLOG_ERROR("解析DoorTile配置失败: {}\n{}", e.what(), door.dump(4));
					}

					SPDLOG_INFO("加载门瓦片[{}]完毕!", id);
				}
				return door_set;
			}

			auto load_tile_set(TileSet& tile_set, const ConfigReader::json_format& json) noexcept -> bool
			{
				SPDLOG_INFO("===========================================");
				SPDLOG_INFO("=============== ->TILE-SET-> ===============");
				SPDLOG_INFO("===========================================");

				if (auto wall_set = load_wall_set(json);
					not wall_set.has_value())
				{
					return false;
				}
				else
				{
					tile_set.wall_tiles = *std::move(wall_set);
				}

				if (auto floor_set = load_floor_set(json);
					not floor_set.has_value())
				{
					return false;
				}
				else
				{
					tile_set.floor_tiles = *std::move(floor_set);
				}

				if (auto decoration_set = load_decoration_set(json);
					not decoration_set.has_value())
				{
					return false;
				}
				else
				{
					tile_set.decoration_tiles = *std::move(decoration_set);
				}

				if (auto trigger_set = load_trigger_set(json);
					not trigger_set.has_value())
				{
					return false;
				}
				else
				{
					tile_set.trigger_tiles = *std::move(trigger_set);
				}

				if (auto door_set = load_door_set(json);
					not door_set.has_value())
				{
					return false;
				}
				else
				{
					tile_set.door_tiles = *std::move(door_set);
				}

				SPDLOG_INFO("===========================================");
				SPDLOG_INFO("=============== <-TILE-SET<- ===============");
				SPDLOG_INFO("===========================================");

				return true;
			}
		} // namespace from_json

		//
	}

	auto load_tile_set_from_json(TileSet& tile_set, const ConfigReader::json_format& json) noexcept -> bool
	{
		if (json.is_string())
		{
			// todo: 链接支持其他格式

			// json
			{
				// "tiles": "/path/to/tile-set-config.json"
				const auto& config_path = json.get_ref<const std::string&>();
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("TileSet的配置文件[{}]加载失败!", config_path);
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
