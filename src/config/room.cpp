// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/room.hpp>

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

namespace pd::config
{
	namespace
	{
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

		namespace from_json
		{
			[[nodiscard]] auto load_room(Room& room, const ConfigReader::json_format& json) noexcept -> bool
			{
				try
				{
					room.name = json["name"].get<std::string>();
					{
						const auto& type_str = json["type"].get_ref<const std::string&>();
						room.type = parse_room_type(type_str);
					}
					room.tile_size = json["tile_size"].get<int>();
					room.width = json["width"].get<int>();
					room.height = json["height"].get<int>();

					// tiles
					{
						const auto do_load_tiles = [&](const std::string_view name, const std::string_view key, auto& dest) noexcept -> bool
						{
							// "xxx": { "name": [ { "x":0, "y": 0 }, ... ] }
							const auto& data = json[key];
							if (not data.is_object())
							{
								SPDLOG_ERROR("房间[{}]的{}配置格式错误!\n{}", room.name, name, json.dump(4));
								return false;
							}

							const auto total_count = std::ranges::fold_left(
								data.items(),
								std::size_t{0},
								[](const std::size_t total, const auto& item) noexcept -> std::size_t
								{
									return total + item.value().size();
								}
							);
							dest.reserve(total_count);

							for (const auto& [tile_id, items]: data.items())
							{
								for (const auto& item: items)
								{
									Position item_position{.x = item["x"].get<int>(), .y = item["y"].get<int>()};

									dest.emplace(item_position, tile_id);
								}
							}

							return true;
						};

						// wall_tiles
						if (not do_load_tiles("墙壁", "walls", room.wall_tiles))
						{
							return false;
						}
						// floor_tiles
						if (not do_load_tiles("地板", "floors", room.floor_tiles))
						{
							return false;
						}
						// decoration_tiles
						if (not do_load_tiles("装饰物", "decorations", room.decoration_tiles))
						{
							return false;
						}
						// trigger_tiles
						if (not do_load_tiles("触发器", "triggers", room.trigger_tiles))
						{
							return false;
						}
					}

					// doors
					{
						// "doors": { "name": [ { "x": 0, "y": 0, "target_room": "room-id", "key": "key-name", "direction": "UP" }, ... ] }
						const auto& doors = json["doors"];
						if (not doors.is_object())
						{
							SPDLOG_ERROR("房间[{}]的门配置格式错误!\n{}", room.name, doors.dump(4));
							return false;
						}

						const auto total_count = std::ranges::fold_left(
							doors.items(),
							std::size_t{0},
							[](const std::size_t total, const auto& item) noexcept -> std::size_t
							{
								return total + item.value().size();
							}
						);
						room.door_tiles.reserve(total_count);

						for (const auto& [tile_id, items]: doors.items())
						{
							for (const auto& item: items)
							{
								Position item_position{.x = item["x"].get<int>(), .y = item["y"].get<int>()};
								auto item_target_room = item["target_room"].get<std::string>();
								auto item_key = item.value("key", "");
								auto item_direction_str = item["direction"].get<std::string>();
								const auto item_direction = parse_door_direction(item_direction_str);

								room.door_tiles.emplace(
									item_position,
									Door
									{
											.tile_id = tile_id,
											.target_room = std::move(item_target_room),
											.key = std::move(item_key),
											.direction = item_direction
									}
								);
							}
						}
					}

					// player_spawn_point
					if (room.type == RoomType::STARTING)
					{
						const auto& player_spawn_point = json["player_spawn_point"];

						room.player_spawn_point.x = player_spawn_point["x"].get<int>();
						room.player_spawn_point.y = player_spawn_point["y"].get<int>();
					}

					// enemy_spawn_points
					if (const auto it = json.find("enemy_spawn_points");
						it != json.end())
					{
						const auto& enemy_spawn_points = it.value();
						room.enemy_spawn_points.reserve(enemy_spawn_points.size());

						for (const auto& item: enemy_spawn_points)
						{
							Position item_position{.x = item["x"].get<int>(), .y = item["y"].get<int>()};

							room.enemy_spawn_points.emplace_back(item_position);
						}
					}

					// npc_spawn_points
					if (const auto it = json.find("npc_spawn_points");
						it != json.end())
					{
						const auto& npc_spawn_points = it.value();
						room.npc_spawn_points.reserve(npc_spawn_points.size());

						for (const auto& item: npc_spawn_points)
						{
							Position item_position{.x = item["x"].get<int>(), .y = item["y"].get<int>()};

							room.npc_spawn_points.emplace_back(item_position);
						}
					}

					// item_spawn_points
					if (const auto it = json.find("item_spawn_points");
						it != json.end())
					{
						const auto& item_spawn_points = it.value();
						room.item_spawn_points.reserve(item_spawn_points.size());

						for (const auto& item: item_spawn_points)
						{
							Position item_position{.x = item["x"].get<int>(), .y = item["y"].get<int>()};

							room.item_spawn_points.emplace_back(item_position);
						}
					}

					return true;
				}
				catch (const nlohmann::json::exception& e)
				{
					SPDLOG_ERROR("解析房间配置时发生错误:{}", e.what());
					return false;
				}
			}

			[[nodiscard]] auto load_room_set(RoomSet& room_set, const ConfigReader::json_format& json) noexcept -> bool
			{
				SPDLOG_INFO("===========================================");
				SPDLOG_INFO("=============== ->ROOM-SET-> ===============");
				SPDLOG_INFO("===========================================");

				room_set.reserve(json.size());

				for (const auto& [id, data]: json.items())
				{
					SPDLOG_INFO("加载房间[{}]...", id);

					Room room{};
					// 调用完整接口,如此即使后续支持格式有变,此处也无需改动
					if (not load_room_from_json(room, data))
					{
						SPDLOG_ERROR("加载房间[{}]失败!", id);
						continue;
					}

					room_set.emplace(id, std::move(room));

					SPDLOG_INFO("加载房间[{}]完毕!", id);
				}

				SPDLOG_INFO("===========================================");
				SPDLOG_INFO("=============== <-ROOM-SET<- ===============");
				SPDLOG_INFO("===========================================");

				return true;
			}
		}

		//
	}

	auto load_room_from_json(Room& room, const ConfigReader::json_format& json) noexcept -> bool
	{
		// 先检查是不是链接
		if (json.is_string())
		{
			// todo: 链接支持其他格式

			// json
			{
				// "room-id": "/path/to/room-config.json"
				const auto& config_path = json.get_ref<const std::string&>();
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("房间的配置文件[{}]加载失败!", config_path);
					return false;
				}

				return from_json::load_room(room, *config);
			}
		}

		return from_json::load_room(room, json);
	}

	auto load_room_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<Room>
	{
		Room room{};
		if (not load_room_from_json(room, json))
		{
			return std::nullopt;
		}

		return room;
	}

	auto load_room_set_from_json(RoomSet& room_set, const ConfigReader::json_format& json) noexcept -> bool
	{
		const auto rooms_it = json.find("rooms");
		if (rooms_it == json.end())
		{
			SPDLOG_ERROR("RoomSet配置格式错误: 缺少'rooms'字段!\n{}", json.dump(4));
			return false;
		}

		const auto& rooms = rooms_it.value();

		if (rooms.is_string())
		{
			// todo: 链接支持其他格式

			// json
			{
				// "rooms": "/path/to/rooms.json"
				const auto& config_path = rooms.get_ref<const std::string&>();
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("RoomSet的配置文件[{}]加载失败!", config_path);
					return false;
				}

				return from_json::load_room_set(room_set, *config);
			}
		}

		return from_json::load_room_set(room_set, rooms);
	}

	auto load_room_set_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<RoomSet>
	{
		RoomSet room_set{};
		if (not load_room_set_from_json(room_set, json))
		{
			return std::nullopt;
		}

		return room_set;
	}
}
