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
					level.name = json["name"].get<std::string>();

					// animation_set
					{
						const auto animations_it = json.find("animations");
						if (animations_it == json.end())
						{
							SPDLOG_ERROR("AnimationSet配置格式错误: 缺少'animations'字段!\n{}", json.dump(4));
							return false;
						}

						const auto& animations = animations_it.value();

						if (auto animation_set = load_animation_set_from_json(animations);
							not animation_set.has_value())
						{
							SPDLOG_ERROR("关卡[{}]的动画集加载失败!", level.name);
							return false;
						}
						else
						{
							level.animation_set = *std::move(animation_set);
						}
					}

					// tile_set
					{
						const auto& tiles_it = json.find("tiles");
						if (tiles_it == json.end())
						{
							SPDLOG_ERROR("TileSet配置格式错误: 缺少'tiles'字段!\n{}", json.dump(4));
							return false;
						}

						const auto& tiles = tiles_it.value();

						if (auto tile_set = load_tile_set_from_json(tiles);
							not tile_set.has_value())
						{
							SPDLOG_ERROR("关卡[{}]的瓦片集加载失败!", level.name);
							return false;
						}
						else
						{
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
								const auto& first_frame = animation.frames.front();

								collision.collision_width = first_frame.texture_width;
								collision.collision_height = first_frame.texture_height;
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
					if (auto room_set = load_room_set_from_json(json);
						not room_set.has_value())
					{
						return false;
					}
					else
					{
						level.room_set = *std::move(room_set);
					}

					// starting_room
					{
						level.starting_room = json["starting_room"].get<std::string>();

						if (const auto it = level.room_set.find(level.starting_room);
							it == level.room_set.end())
						{
							SPDLOG_ERROR("加载关卡[{}]的初始房间失败!初始房间[{}]不存在!", level.name, level.starting_room);
							return false;
						}
					}

					return true;
				}
				catch (const nlohmann::json::exception& e)
				{
					SPDLOG_ERROR("解析关卡配置时发生错误:{}", e.what());
					return false;
				}
			}

			auto load_level_set(LevelSet& level_set, const ConfigReader::json_format& json) noexcept -> bool
			{
				SPDLOG_INFO("===========================================");
				SPDLOG_INFO("=============== ->LEVEL-SET-> ===============");
				SPDLOG_INFO("===========================================");

				level_set.reserve(json.size());

				for (const auto& [id, data]: json.items())
				{
					SPDLOG_INFO("加载关卡[{}]...", id);

					Level level{};
					// 调用完整接口,如此即使后续支持格式有变,此处也无需改动
					if (not load_level_from_json(level, data))
					{
						SPDLOG_ERROR("加载关卡[{}]失败!", id);
						continue;
					}

					level_set.emplace(id, std::move(level));

					SPDLOG_INFO("加载关卡[{}]完毕!", id);
				}

				SPDLOG_INFO("===========================================");
				SPDLOG_INFO("=============== <-LEVEL-SET<- ===============");
				SPDLOG_INFO("===========================================");

				return true;
			}
		}

		//
	}

	auto load_level_from_json(Level& level, const ConfigReader::json_format& json) noexcept -> bool
	{
		// 先检查是不是链接
		if (json.is_string())
		{
			// todo: 链接支持其他格式

			// json
			{
				// "level-id": "/path/to/level-config.json"
				const auto& config_path = json.get_ref<const std::string&>();
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("关卡的配置文件[{}]加载失败!", config_path);
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
		const auto levels_it = json.find("levels");
		if (levels_it == json.end())
		{
			SPDLOG_ERROR("关卡集配置格式错误: 缺少'levels'字段!");
			return false;
		}

		const auto& levels = levels_it.value();

		if (levels.is_string())
		{
			// todo: 链接支持其他格式

			// json
			{
				// "levels": "/path/to/levels.json"
				const auto& config_path = levels.get_ref<const std::string&>();
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("LevelSet的配置文件[{}]加载失败!", config_path);
					return false;
				}

				return from_json::load_level_set(level_set, *config);
			}
		}

		return from_json::load_level_set(level_set, levels);
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
