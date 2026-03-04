// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/level.hpp>

#include <algorithm>
#include <ranges>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace pd::config
{
	namespace from_json
	{
		// 支持其他格式后,这些接口其他文件也许用得到?先不要放到匿名命名空间了
		extern auto load_level(Level& level, const ConfigReader::json_format& json) noexcept -> bool;
		extern auto load_level_set(LevelSet& level_set, const ConfigReader::json_format& json) noexcept -> bool;

		[[nodiscard]] auto load_level(Level& level, const ConfigReader::json_format& json) noexcept -> bool
		{
			try
			{
				// name
				{
					const auto name_it = json.find("name");
					if (name_it == json.end())
					{
						SPDLOG_ERROR("关卡配置格式错误: 缺少'name'字段!\n{}", json.dump(4));
						return false;
					}

					const auto& name = name_it.value();
					level.name = name.get<std::string>();
					SPDLOG_INFO("读取到关卡名称: [{}]...", level.name);
				}

				// animation_set
				{
					SPDLOG_INFO("\n==| 正在加载动画集数据 |==");

					const auto animations_it = json.find("animations");
					if (animations_it == json.end())
					{
						SPDLOG_ERROR("关卡配置格式错误: 缺少'animations'字段!\n{}", json.dump(4));
						return false;
					}

					const auto& animations = animations_it.value();
					if (auto animation_set = load_animation_set_from_json(animations);
						not animation_set.has_value())
					{
						SPDLOG_INFO("\n==| 动画集数据加载失败 |==");
						return false;
					}
					else
					{
						SPDLOG_INFO("\n==| 动画集数据加载成功 |==");
						level.animation_set = *std::move(animation_set);
					}
				}

				// tile_set
				{
					SPDLOG_INFO("\n==| 正在加载瓦片集数据 |==");

					const auto& tiles_it = json.find("tiles");
					if (tiles_it == json.end())
					{
						SPDLOG_ERROR("关卡配置格式错误: 缺少'tiles'字段!\n{}", json.dump(4));
						return false;
					}

					const auto& tiles = tiles_it.value();
					if (auto tile_set = load_tile_set_from_json(tiles); not tile_set.has_value())
					{
						SPDLOG_INFO("\n==| 瓦片集数据加载失败 |==");
						return false;
					}
					else
					{
						SPDLOG_INFO("\n==| 瓦片集数据加载成功 |==");
						level.tile_set = *std::move(tile_set);

						// 检查所有瓦片的动画是否存在,并且设置那些具有Collision的瓦片的collision_width/collision_height(如果它们未设置的话)
						const auto do_process_tile = [&]<typename Tile>(Set<Tile>& target_tiles) noexcept -> bool
						{
							for (auto& [tile_id, tile]: target_tiles)
							{
								// tile.hpp --> detail::Animation::animation_id
								const auto animation_it = level.animation_set.find(tile.animation_id);
								if (animation_it == level.animation_set.end())
								{
									SPDLOG_ERROR("瓦片[{}]的动画[{}]不存在!", tile_id, tile.animation_id);
									return false;
								}

								// tile.hpp --> detail::Collision::collision_width/collision_height
								if constexpr (requires { tile.collision_width; tile.collision_height; })
								{
									const auto& animation = animation_it->second;

									if (tile.collision_width == 0)
									{
										tile.collision_width = animation.texture_width;
									}
									if (tile.collision_height == 0)
									{
										tile.collision_height = animation.texture_height;
									}
								}
							}

							return true;
						};

						if (not do_process_tile(level.tile_set.wall_tiles))
						{
							return false;
						}
						if (not do_process_tile(level.tile_set.floor_tiles))
						{
							return false;
						}
						if (not do_process_tile(level.tile_set.decoration_tiles))
						{
							return false;
						}
						if (not do_process_tile(level.tile_set.trigger_tiles))
						{
							return false;
						}
						if (not do_process_tile(level.tile_set.key_tiles))
						{
							return false;
						}
						if (not do_process_tile(level.tile_set.door_tiles))
						{
							return false;
						}
					}
				}

				// player_set
				{
					SPDLOG_INFO("\n==| 正在加载玩家集数据 |==");

					const auto players_it = json.find("players");
					if (players_it == json.end())
					{
						SPDLOG_ERROR("玩家配置格式错误: 缺少'animations'字段!\n{}", json.dump(4));
						return false;
					}

					const auto& players = players_it.value();
					if (auto player_set = load_player_set_from_json(players);
						not player_set.has_value())
					{
						SPDLOG_INFO("\n==| 玩家集数据加载失败 |==");
						return false;
					}
					else
					{
						SPDLOG_INFO("\n==| 玩家集数据加载成功 |==");
						level.player_set = *std::move(player_set);
					}

					// todo: 检查敌人的武器是否在武器集中存在
				}

				// player_id
				{
					const auto player_id_it = json.find("player_id");
					if (player_id_it == json.end())
					{
						SPDLOG_ERROR("关卡的配置格式错误: 缺少'player_id'字段!\n{}", json.dump(4));
						return false;
					}

					const auto& player_id = player_id_it.value();
					level.player_id = player_id.get<std::string>();

					if (not level.player_set.contains(level.player_id))
					{
						SPDLOG_ERROR("关卡的玩家错误!玩家[{}]不存在!", level.player_id);
						return false;
					}
				}

				// enemy_set
				{
					SPDLOG_INFO("\n==| 正在加载敌人集数据 |==");

					const auto enemies_it = json.find("enemies");
					if (enemies_it == json.end())
					{
						SPDLOG_ERROR("玩家配置格式错误: 缺少'enemies'字段!\n{}", json.dump(4));
						return false;
					}

					const auto& enemies = enemies_it.value();
					if (auto enemy_set = load_enemy_set_from_json(enemies);
						not enemy_set.has_value())
					{
						SPDLOG_INFO("\n==| 敌人集数据加载失败 |==");
						return false;
					}
					else
					{
						SPDLOG_INFO("\n==| 敌人集数据加载成功 |==");
						level.enemy_set = *std::move(enemy_set);
					}

					// todo: 检查敌人的武器是否在武器集中存在
				}

				// room_set
				{
					SPDLOG_INFO("\n==| 正在加载房间集数据 |==");

					const auto& rooms_it = json.find("rooms");
					if (rooms_it == json.end())
					{
						SPDLOG_ERROR("关卡配置格式错误: 缺少'rooms'字段!\n{}", json.dump(4));
						return false;
					}

					const auto& rooms = rooms_it.value();
					if (auto room_set = load_room_set_from_json(rooms);
						not room_set.has_value())
					{
						SPDLOG_INFO("\n==| 房间集数据加载失败 |==");
						return false;
					}
					else
					{
						SPDLOG_INFO("\n==| 房间集数据加载成功 |==");
						level.room_set = *std::move(room_set);

						// 检查所有房间的门的目标房间是否存在
						// 检查所有房间的门的钥匙是否存在(如果门需要钥匙的话)
						const auto do_check_door = [&](
							const std::string_view room_id,
							const std::array<std::optional<std::pair<Position, Door>>, 4>& doors,
							const DoorDirection direction
						) noexcept -> bool
						{
							const auto& door_opt = doors[index_of(direction)];
							if (not door_opt.has_value())
							{
								return true;
							}

							const auto& door = door_opt->second;

							if (not level.room_set.contains(door.target_room))
							{
								SPDLOG_ERROR("房间[{}]的门的目标房间[{}]不存在!", room_id, door.target_room);
								return false;
							}

							if (not door.key.empty())
							{
								const auto& room_set_rooms = level.room_set | std::views::values;

								// 检查是否存在需要钥匙且钥匙名称匹配的房间
								if (const auto it = std::ranges::find_if(
										room_set_rooms,
										[&](const Room& room) noexcept -> bool
										{
											// 不需要钥匙
											if (not room.key_tile.has_value())
											{
												return false;
											}

											// 比对钥匙名称
											const auto& key_tile = room.key_tile->second;
											return key_tile.name == door.key;
										}
									);
									it == room_set_rooms.end())
								{
									SPDLOG_ERROR("房间[{}]的钥匙[{}]不属于任何房间!", room_id, door.key);
									return false;
								}
							}

							return true;
						};

						for (const auto& [room_id, room]: level.room_set)
						{
							do_check_door(room_id, room.door_tiles, DoorDirection::UP);
							do_check_door(room_id, room.door_tiles, DoorDirection::DOWN);
							do_check_door(room_id, room.door_tiles, DoorDirection::LEFT);
							do_check_door(room_id, room.door_tiles, DoorDirection::RIGHT);
						}
					}
				}

				// starting_room
				{
					const auto starting_room_it = json.find("starting_room");
					if (starting_room_it == json.end())
					{
						SPDLOG_ERROR("关卡的配置格式错误: 缺少'starting_room'字段!\n{}", json.dump(4));
						return false;
					}

					const auto& starting_room = starting_room_it.value();
					level.starting_room = starting_room.get<std::string>();

					if (const auto it = level.room_set.find(level.starting_room);
						it == level.room_set.end())
					{
						SPDLOG_ERROR("关卡的初始房间错误!初始房间[{}]不存在!", level.starting_room);
						return false;
					}
					else if (it->second.type != RoomType::STARTING)
					{
						SPDLOG_ERROR("关卡的初始房间错误![{}]房间的类型不正确!", level.starting_room);
						return false;
					}
				}

				return true;
			}
			catch (const nlohmann::json::exception& e)
			{
				SPDLOG_ERROR("解析关卡配置时发生错误: {}\n{}", e.what(), json.dump(4));
				return false;
			}
		}

		[[nodiscard]] auto load_level_set(LevelSet& level_set, const ConfigReader::json_format& json) noexcept -> bool
		{
			level_set.reserve(json.size());

			for (const auto& [id, data]: json.items())
			{
				SPDLOG_INFO("\n======== 正在加载关卡[{}]数据 ========\n", id);

				Level level{};
				// 调用完整接口,如此即使后续支持格式有变,此处也无需改动
				if (not load_level_from_json(level, data))
				{
					SPDLOG_ERROR("\n======== 加载关卡[{}]失败 ========\n", id);
					continue;
				}

				level_set.emplace(id, std::move(level));

				SPDLOG_INFO("\n======== 加载关卡[{}]完毕 ========\n", id);
			}

			return true;
		}
	} // namespace from_json

	auto load_level_from_json(Level& level, const ConfigReader::json_format& json) noexcept -> bool
	{
		// "level-id": "/path/to/level.xxx"
		if (json.is_string())
		{
			const auto& config_path = json.get_ref<const std::string&>();
			SPDLOG_INFO("正在加载关卡指定配置文件[{}]...", config_path);

			// todo: 链接支持其他格式

			// "level-id": "/path/to/level.json"
			{
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("关卡指定的配置文件[{}]加载失败!", config_path);
					return false;
				}

				return from_json::load_level(level, *config);
			}
		}

		return from_json::load_level(level, json);
	}

	auto load_level_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<Level>
	{
		Level level{};
		if (not load_level_from_json(level, json))
		{
			return std::nullopt;
		}

		return level;
	}

	auto load_level_set_from_json(LevelSet& level_set, const ConfigReader::json_format& json) noexcept -> bool
	{
		SPDLOG_INFO("正在加载关卡集数据...");

		// "levels": "/path/to/levels.xxx"
		if (json.is_string())
		{
			const auto& config_path = json.get_ref<const std::string&>();
			SPDLOG_INFO("正在加载关卡集指定配置文件[{}]...", config_path);

			// todo: 链接支持其他格式

			// "levels": "/path/to/levels.json"
			{
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("关卡集指定的配置文件[{}]加载失败!", config_path);
					return false;
				}

				return from_json::load_level_set(level_set, *config);
			}
		}

		return from_json::load_level_set(level_set, json);
	}

	auto load_level_set_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<LevelSet>
	{
		LevelSet level_set{};
		if (not load_level_set_from_json(level_set, json))
		{
			return std::nullopt;
		}

		return level_set;
	}
} // namespace pd::config
