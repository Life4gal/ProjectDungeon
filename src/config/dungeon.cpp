// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/dungeon.hpp>

#include <utility/json_reader.hpp>

#include <spdlog/spdlog.h>

namespace pd::config
{
	namespace
	{
		// 解析TileType枚举
		auto parse_tile_type(const std::string& type_str) noexcept -> TileType
		{
			if (type_str == "FLOOR")
			{
				return TileType::FLOOR;
			}
			if (type_str == "WALL")
			{
				return TileType::WALL;
			}
			if (type_str == "WALL_DECORATION")
			{
				return TileType::WALL_DECORATION;
			}
			if (type_str == "FLOOR_DECORATION")
			{
				return TileType::FLOOR_DECORATION;
			}
			if (type_str == "LOW_TERRAIN")
			{
				return TileType::LOW_TERRAIN;
			}
			if (type_str == "HIGH_TERRAIN")
			{
				return TileType::HIGH_TERRAIN;
			}

			SPDLOG_WARN("未知的瓦片类型:{},默认使用FLOOR", type_str);
			return TileType::FLOOR;
		}

		// 解析RoomType枚举
		auto parse_room_type(const std::string& type_str) noexcept -> RoomType
		{
			if (type_str == "STARTING")
			{
				return RoomType::STARTING;
			}
			if (type_str == "NORMAL")
			{
				return RoomType::NORMAL;
			}
			if (type_str == "TREASURE")
			{
				return RoomType::TREASURE;
			}
			if (type_str == "KEY")
			{
				return RoomType::KEY;
			}
			if (type_str == "BOSS")
			{
				return RoomType::BOSS;
			}
			if (type_str == "EXIT")
			{
				return RoomType::EXIT;
			}

			SPDLOG_WARN("未知的房间类型:{},默认使用NORMAL", type_str);
			return RoomType::NORMAL;
		}

		// 解析DoorDirection枚举
		auto parse_door_direction(const std::string& direction_str) noexcept -> DoorDirection
		{
			if (direction_str == "UP")
			{
				return DoorDirection::UP;
			}
			if (direction_str == "DOWN")
			{
				return DoorDirection::DOWN;
			}
			if (direction_str == "LEFT")
			{
				return DoorDirection::LEFT;
			}
			if (direction_str == "RIGHT")
			{
				return DoorDirection::RIGHT;
			}

			SPDLOG_WARN("未知的门方向:{},默认使用DOWN", direction_str);
			return DoorDirection::DOWN;
		}
	}

	namespace
	{
		auto load_tileset_from_json(const nlohmann::json& json) noexcept -> TileSet
		{
			TileSet tileset{};

			try
			{
				if (not json.contains("tiles"))
				{
					SPDLOG_ERROR("瓦片集配置格式错误:缺少'tiles'字段!");
					return tileset;
				}

				for (const auto& tiles = json["tiles"];
				     const auto& [tile_name, tile_data]: tiles.items())
				{
					Tile tile{};

					tile.texture_path = tile_data["texture_path"].get<std::string>();
					tile.texture_x = tile_data["texture_x"].get<int>();
					tile.texture_y = tile_data["texture_y"].get<int>();
					tile.texture_width = tile_data["texture_width"].get<int>();
					tile.texture_height = tile_data["texture_height"].get<int>();

					const auto type_str = tile_data["type"].get<std::string>();
					tile.type = parse_tile_type(type_str);

					SPDLOG_DEBUG("加载瓦片[{}]:{}x{}", tile_name, tile.width, tile.height);
					tileset.emplace(tile_name, std::move(tile));
				}

				SPDLOG_INFO("加载瓦片集成功!共有[{}]个瓦片", tileset.size());
			}
			catch (const nlohmann::json::exception& e)
			{
				SPDLOG_ERROR("解析瓦片集配置时发生错误:{}", e.what());
			}

			return tileset;
		}
	}

	auto load_tileset(const std::filesystem::path& path) noexcept -> TileSet
	{
		const auto json_opt = utility::JsonReader::read_file(path);
		if (not json_opt.has_value())
		{
			return {};
		}

		const auto& json = json_opt.value();
		return load_tileset_from_json(json);
	}

	namespace
	{
		auto load_room_from_json(const nlohmann::json& json) noexcept -> Room
		{
			Room room{};

			try
			{
				room.id = json["id"].get<std::string>();

				const auto type_str = json["type"].get<std::string>();
				room.room_type = parse_room_type(type_str);

				room.width = json["width"].get<int>();
				room.height = json["height"].get<int>();
				room.tile_size = json["tile_size"].get<int>();

				// 解析瓦片布局
				if (json.contains("layout"))
				{
					const auto& layout = json["layout"];
					room.layout.reserve(layout.size());

					for (const auto& row: layout)
					{
						room.layout.emplace_back(row.get<std::string>());
					}
				}

				// 解析字符到瓦片ID的映射
				if (json.contains("tile_mapping"))
				{
					for (const auto& mapping = json["tile_mapping"];
					     const auto& [char_str, tile_id]: mapping.items())
					{
						if (not char_str.empty())
						{
							room.tile_mapping[char_str[0]] = tile_id.get<std::string>();
						}
					}
				}

				// 解析门配置
				if (json.contains("doors"))
				{
					const auto& doors = json["doors"];
					room.doors.reserve(doors.size());

					for (const auto& door_data: doors)
					{
						DoorConfig door{};

						door.target_room_id = door_data["target_room"].get<std::string>();
						door.tile_id = door_data["tile_id"].get<std::string>();
						door.tile_x = door_data["tile_x"].get<int>();
						door.tile_y = door_data["tile_y"].get<int>();

						const auto direction_str = door_data["direction"].get<std::string>();
						door.direction = parse_door_direction(direction_str);

						door.requires_key = door_data.value("requires_key", false);

						room.doors.emplace_back(std::move(door));
					}
				}

				// 解析敌人生成点
				if (json.contains("enemy_spawns"))
				{
					const auto& spawns = json["enemy_spawns"];
					room.enemy_spawns.reserve(spawns.size());

					for (const auto& spawn_data: spawns)
					{
						EnemySpawn spawn{};

						spawn.enemy_id = spawn_data["enemy_id"].get<std::string>();
						spawn.tile_x = spawn_data["tile_x"].get<int>();
						spawn.tile_y = spawn_data["tile_y"].get<int>();

						room.enemy_spawns.emplace_back(std::move(spawn));
					}
				}

				SPDLOG_INFO("加载房间[{}]成功!尺寸:{}x{}", room.id, room.width, room.height);
			}
			catch (const nlohmann::json::exception& e)
			{
				SPDLOG_ERROR("解析房间配置时发生错误:{}", e.what());
			}

			return room;
		}
	}

	auto load_room(const std::filesystem::path& path) noexcept -> Room
	{
		const auto json_opt = utility::JsonReader::read_file(path);
		if (not json_opt.has_value())
		{
			return {};
		}

		const auto& json = json_opt.value();
		return load_room_from_json(json);
	}

	namespace
	{
		auto load_level_from_json(const nlohmann::json& json) noexcept -> Level
		{
			Level level{};

			try
			{
				level.id = json["id"].get<std::string>();
				level.name = json.value("name", level.id);
				level.starting_room_id = json["starting_room"].get<std::string>();
				level.tileset_path = json.value("tileset", "assets/configs/tileset.json");

				// 加载房间
				if (json.contains("rooms"))
				{
					const auto& rooms = json["rooms"];

					// 如果rooms是数组,每个元素是房间文件路径或内联房间数据
					if (rooms.is_array())
					{
						for (const auto& room_entry: rooms)
						{
							if (room_entry.is_string())
							{
								// 房间文件路径
								// auto room_path = path.parent_path() / room_entry.get<std::string>();
								auto room_path = room_entry.get<std::string>();

								auto room = load_room(room_path);
								// todo: 解析失败?
								level.rooms.emplace(room.id, std::move(room));
							}
							else if (room_entry.is_object())
							{
								// 内联房间数据
								auto room = load_room_from_json(room_entry);
								// todo: 解析失败?
								level.rooms.emplace(room.id, std::move(room));
							}
						}
					}
				}

				SPDLOG_INFO("加载关卡[{}]成功!共有[{}]个房间", level.id, level.rooms.size());
			}
			catch (const nlohmann::json::exception& e)
			{
				SPDLOG_ERROR("解析关卡配置时发生错误:{}", e.what());
			}

			return level;
		}
	}

	auto load_level(const std::filesystem::path& path) noexcept -> Level
	{
		const auto json_opt = utility::JsonReader::read_file(path);
		if (not json_opt.has_value())
		{
			return {};
		}

		const auto& json = json_opt.value();
		return load_level_from_json(json);
	}

	namespace
	{
		auto load_dungeon_from_json(const nlohmann::json& json) noexcept -> Dungeon
		{
			Dungeon dungeon{};

			try
			{
				dungeon.id = json["id"].get<std::string>();
				dungeon.name = json.value("name", dungeon.id);

				// 解析关卡顺序
				if (json.contains("level_order"))
				{
					for (const auto& level_id: json["level_order"])
					{
						dungeon.level_order.emplace_back(level_id.get<std::string>());
					}
				}

				// 加载关卡
				if (json.contains("levels"))
				{
					const auto& levels = json["levels"];

					if (levels.is_array())
					{
						for (const auto& level_entry: levels)
						{
							if (level_entry.is_string())
							{
								// 关卡文件路径
								// auto level_path = path.parent_path() / level_entry.get<std::string>();
								auto level_path = level_entry.get<std::string>();

								auto level = load_level(level_path);
								// todo: 解析失败?
								dungeon.levels.emplace(level.id, std::move(level));
							}
						}
					}
				}

				SPDLOG_INFO("加载地下城[{}]成功!共有[{}]个关卡", dungeon.id, dungeon.levels.size());
			}
			catch (const nlohmann::json::exception& e)
			{
				SPDLOG_ERROR("解析地下城配置时发生错误:{}", e.what());
			}

			return dungeon;
		}
	}

	auto load_dungeon(const std::filesystem::path& path) noexcept -> Dungeon
	{
		const auto json_opt = utility::JsonReader::read_file(path);
		if (not json_opt.has_value())
		{
			return {};
		}

		const auto& json = json_opt.value();
		return load_dungeon_from_json(json);
	}
} // namespace pd::config
