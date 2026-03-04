// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/room.hpp>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace pd::config
{
	namespace from_json
	{
		// 支持其他格式后,这些接口其他文件也许用得到?先不要放到匿名命名空间了
		extern auto load_room(Room& room, const ConfigReader::json_format& json) noexcept -> bool;
		extern auto load_room_set(RoomSet& room_set, const ConfigReader::json_format& json) noexcept -> bool;

		[[nodiscard]] auto load_room(Room& room, const ConfigReader::json_format& json) noexcept -> bool
		{
			try
			{
				// name + tile_size + width + height
				{
					const auto do_load = [&]<typename T>(const std::string_view name, const std::string_view key, T& dest) noexcept -> bool
					{
						const auto it = json.find(key);
						if (it == json.end())
						{
							SPDLOG_ERROR("房间配置格式错误: 缺少'{}'字段!\n{}", key, json.dump(4));
							return false;
						}

						const auto& value = it.value();

						dest = value.get<T>();
						SPDLOG_INFO("读取到房间{}: [{}]", name, dest);
						return true;
					};

					// name
					if (not do_load("名称", "name", room.name))
					{
						return false;
					}
					// tile_size
					if (not do_load("瓦片大小", "tile_size", room.tile_size))
					{
						return false;
					}
					// width
					if (not do_load("房间宽度", "width", room.width))
					{
						return false;
					}
					// height
					if (not do_load("房间高度", "height", room.height))
					{
						return false;
					}
				}
				// type
				{
					const auto type_it = json.find("type");
					if (type_it == json.end())
					{
						SPDLOG_ERROR("房间配置格式错误: 缺少'type'字段!\n{}", json.dump(4));
						return false;
					}

					const auto& type = type_it.value();
					const auto& type_str = type.get_ref<const std::string&>();

					room.type = room_type_of(type_str);
				}

				// tiles
				{
					const auto do_load_tiles = [&]<typename T>(const std::string_view name, const std::string_view key, T& dest) noexcept -> bool
					{
						// "xxx": { "tile-id": [ { "x":0, "y": 0 }, ... ] }
						const auto it = json.find(key);
						if (it == json.end())
						{
							SPDLOG_ERROR("房间配置格式错误: 缺少'{}'字段!\n{}", key, json.dump(4));
							return false;
						}

						const auto& data = it.value();
						if (not data.is_object())
						{
							SPDLOG_ERROR("房间配置格式错误: '{}'的数据类型'{}'错误!\n{}", name, data.type_name(), data.dump(4));
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

				// key
				if (const auto key_it = json.find("key");
					key_it != json.end())
				{
					// "key": { "id": "key-id-1", "x": 0, "y": 0, "name": "key-name" }, 
					const auto& key = key_it.value();
					if (not key.is_object())
					{
						SPDLOG_ERROR("房间配置格式错误: 'key'的数据类型'{}'错误!\n{}", key.type_name(), key.dump(4));
						return false;
					}

					auto item_id = key["id"].get<std::string>();
					Position item_position{.x = key["x"].get<int>(), .y = key["y"].get<int>()};
					auto item_name = key["name"].get<std::string>();

					room.key_tile = std::make_pair(
						item_position,
						Key
						{
								.tile_id = std::move(item_id),
								.name = std::move(item_name)
						}
					);
				}
				else
				{
					// 钥匙是可选的,如果没有配置则表示该房间没有钥匙
					room.key_tile = std::nullopt;
				}

				// doors
				{
					// "doors": { "UP": { "id": "door-id-1", "x": 0, "y": 0, "target_room": "room-id", "key": "key-name" }, "DOWN": { ... }, "LEFT": { ... }, "RIGHT": { ... } },
					const auto doors_it = json.find("doors");
					if (doors_it == json.end())
					{
						SPDLOG_ERROR("房间配置格式错误: 缺少'doors'字段!\n{}", json.dump(4));
						return false;
					}

					const auto& doors = doors_it.value();
					if (not doors.is_object())
					{
						SPDLOG_ERROR("房间配置格式错误: 'doors'的数据类型'{}'错误!\n{}", doors.type_name(), doors.dump(4));
						return false;
					}

					const auto do_load_door = [&](const std::string_view key, const DoorDirection direction) noexcept -> void
					{
						const auto door_it = doors.find(key);
						if (door_it == doors.end())
						{
							return;
						}

						const auto& door = door_it.value();

						auto item_id = door["id"].get<std::string>();
						auto item_position = Position{.x = door["x"].get<int>(), .y = door["y"].get<int>()};
						auto item_target_room = door["target_room"].get<std::string>();
						auto item_key = door.value("key", "");

						room.door_tiles[index_of(direction)] = std::make_pair(
							item_position,
							Door
							{
									.tile_id = std::move(item_id),
									.target_room = std::move(item_target_room),
									.key = std::move(item_key),
									.direction = direction,
							}
						);

						// 检查钥匙是否存在需要交给关卡处理
					};

					do_load_door("UP", DoorDirection::UP);
					do_load_door("DOWN", DoorDirection::DOWN);
					do_load_door("LEFT", DoorDirection::LEFT);
					do_load_door("RIGHT", DoorDirection::RIGHT);

					if (not std::ranges::any_of(
						room.door_tiles,
						[](const auto& tile) noexcept -> bool
						{
							return tile.has_value();
						}
					))
					{
						SPDLOG_ERROR("房间配置内容错误: 没有指定任何门!\n{}", doors.dump(4));
						return false;
					}
				}

				// player_spawn_point
				{
					const auto center_point = Position{.x = room.width / 2, .y = room.height / 2};

					if (room.type == RoomType::STARTING)
					{
						room.player_spawn_point = center_point;
					}
					else
					{
						if (const auto it = json.find("player_spawn_point");
							it != json.end())
						{
							const auto& player_spawn_point = it.value();

							room.player_spawn_point.x = player_spawn_point["x"].get<int>();
							room.player_spawn_point.y = player_spawn_point["y"].get<int>();
						}
						else
						{
							SPDLOG_WARN("非初始房间[{}]未指定玩家生成点,默认使用房间中心", room.name);
							room.player_spawn_point = center_point;
						}
					}
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
				SPDLOG_ERROR("解析房间配置时发生错误: {}\n{}", e.what(), json.dump(4));
				return false;
			}
		}

		[[nodiscard]] auto load_room_set(RoomSet& room_set, const ConfigReader::json_format& json) noexcept -> bool
		{
			room_set.reserve(json.size());

			for (const auto& [id, data]: json.items())
			{
				SPDLOG_INFO("正在加载房间[{}]数据...", id);

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

			return true;
		}
	} // namespace from_json

	auto load_room_from_json(Room& room, const ConfigReader::json_format& json) noexcept -> bool
	{
		// "room-id": "/path/to/room-config.xxx"
		if (json.is_string())
		{
			const auto& config_path = json.get_ref<const std::string&>();
			SPDLOG_INFO("正在加载房间指定配置文件[{}]...", config_path);

			// todo: 链接支持其他格式

			// "room-id": "/path/to/room-config.json"
			{
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("房间指定的配置文件[{}]加载失败!", config_path);
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
		// "rooms": /path/to/rooms.xxx"
		if (json.is_string())
		{
			const auto& config_path = json.get_ref<const std::string&>();
			SPDLOG_INFO("正在加载房间集指定配置文件[{}]...", config_path);

			// todo: 链接支持其他格式

			// "rooms": "/path/to/rooms.json"
			{
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("房间集指定的配置文件[{}]加载失败!", config_path);
					return false;
				}

				return from_json::load_room_set(room_set, *config);
			}
		}

		return from_json::load_room_set(room_set, json);
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
} // namespace pd::config
