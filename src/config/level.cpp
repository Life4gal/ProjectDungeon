// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/level.hpp>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace pd::config
{
	namespace
	{
		namespace from_json
		{
			auto load_level(Level& level, const ConfigReader::json_format& json) noexcept -> bool
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
						if (auto tile_set = load_tile_set_from_json(tiles);
							not tile_set.has_value())
						{
							SPDLOG_INFO("\n==| 瓦片集数据加载失败 |==");
							return false;
						}
						else
						{
							SPDLOG_INFO("\n==| 瓦片集数据加载成功 |==");
							level.tile_set = *std::move(tile_set);

							// 对于那些具有Collision的瓦片,我们需要设置其collision_width/collision_height(如果它们未设置)
							const auto do_set_tile_collision = [&](const std::string_view tile_id, const std::string_view animation_id, detail::Collision& collision) noexcept -> bool
							{
								if (collision.collision_width != 0 and collision.collision_height != 0)
								{
									return true;
								}

								const auto animation_it = level.animation_set.find(animation_id);
								if (animation_it == level.animation_set.end())
								{
									SPDLOG_ERROR("瓦片[{}]的动画[{}]不存在!", tile_id, animation_id);
									return false;
								}

								const auto& animation = animation_it->second;

								collision.collision_width = animation.texture_width;
								collision.collision_height = animation.texture_height;
								return true;
							};

							// wall_tiles
							for (auto& [id, tile]: level.tile_set.wall_tiles)
							{
								do_set_tile_collision(id, tile.animation_id, tile);
							}
							// trigger_tiles
							for (auto& [id, tile]: level.tile_set.trigger_tiles)
							{
								do_set_tile_collision(id, tile.animation_id, tile);
							}
							// door_tiles
							for (auto& [id, tile]: level.tile_set.door_tiles)
							{
								do_set_tile_collision(id, tile.animation_id, tile);
							}
						}
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
							for (const auto& [room_id, room]: level.room_set)
							{
								for (const auto& door: room.door_tiles | std::views::values)
								{
									if (const auto it = level.room_set.find(door.target_room);
										it == level.room_set.end())
									{
										SPDLOG_ERROR("房间[{}]的门的目标房间[{}]不存在!", room_id, door.target_room);
										return false;
									}
								}
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

			auto load_level_set(LevelSet& level_set, const ConfigReader::json_format& json) noexcept -> bool
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
		}

		//
	}

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
}
