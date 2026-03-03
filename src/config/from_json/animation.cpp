// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/animation.hpp>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace pd::config
{
	namespace from_json
	{
		// 支持其他格式后,这些接口其他文件也许用得到?先不要放到匿名命名空间了
		extern auto load_animation_frame(AnimationFrame& frame, const ConfigReader::json_format& json) noexcept -> bool;
		extern auto load_animation(Animation& animation, const ConfigReader::json_format& json) noexcept -> bool;
		extern auto load_animation_set(AnimationSet& animation_set, const ConfigReader::json_format& json) noexcept -> bool;

		[[nodiscard]] auto load_animation_frame(AnimationFrame& frame, const ConfigReader::json_format& json) noexcept -> bool
		{
			try
			{
				frame.texture_path = json["texture_path"].get<std::string>();
				frame.texture_x = json["texture_x"].get<int>();
				frame.texture_y = json["texture_y"].get<int>();
				frame.duration_ms = json["duration_ms"].get<int>();

				return true;
			}
			catch (const nlohmann::json::exception& e)
			{
				SPDLOG_ERROR("解析动画帧配置时发生错误: {}", e.what());
				return false;
			}
		}

		[[nodiscard]] auto load_animation(Animation& animation, const ConfigReader::json_format& json) noexcept -> bool
		{
			try
			{
				// frames
				{
					const auto frames_it = json.find("frames");
					if (frames_it == json.end())
					{
						SPDLOG_ERROR("动画配置格式错误: 缺少'frames'字段!\n{}", json.dump(4));
						return false;
					}

					const auto& frames = frames_it.value();
					if (frames.empty())
					{
						SPDLOG_ERROR("动画配置格式错误: 'frames'字段内容为空!\n{}", json.dump(4));
						return false;
					}

					animation.frames.reserve(json.size());
					for (const auto& data: frames)
					{
						AnimationFrame frame{};
						// 调用完整接口,如此即使后续支持格式有变,此处也无需改动
						if (not load_animation_frame_from_json(frame, data))
						{
							return false;
						}

						animation.frames.emplace_back(std::move(frame));
					}
				}

				// width/height
				animation.texture_width = json["texture_width"].get<int>();
				animation.texture_height = json["texture_height"].get<int>();

				// origin
				animation.origin_x = json.value("origin_x", animation.texture_width / 2);
				animation.origin_y = json.value("origin_y", animation.texture_height / 2);

				// looping
				animation.looping = json.value("looping", false);

				return true;
			}
			catch (const nlohmann::json::exception& e)
			{
				SPDLOG_ERROR("解析动画配置时发生错误: {}", e.what());
				return false;
			}
		}

		[[nodiscard]] auto load_animation_set(AnimationSet& animation_set, const ConfigReader::json_format& json) noexcept -> bool
		{
			animation_set.reserve(json.size());

			for (const auto& [id, data]: json.items())
			{
				SPDLOG_INFO("正在加载动画[{}]数据...", id);

				Animation animation{};
				// 调用完整接口,如此即使后续支持格式有变,此处也无需改动
				if (not load_animation_from_json(animation, data))
				{
					SPDLOG_ERROR("加载动画[{}]失败!", id);
					continue;
				}

				animation_set.emplace(id, std::move(animation));

				SPDLOG_INFO("加载动画[{}]]完毕!", id);
			}

			return true;
		}
	}

	auto load_animation_frame_from_json(AnimationFrame& frame, const ConfigReader::json_format& json) noexcept -> bool
	{
		return from_json::load_animation_frame(frame, json);
	}

	auto load_animation_frame_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<AnimationFrame>
	{
		AnimationFrame frame{};
		if (not load_animation_frame_from_json(frame, json))
		{
			return std::nullopt;
		}

		return frame;
	}

	auto load_animation_from_json(Animation& animation, const ConfigReader::json_format& json) noexcept -> bool
	{
		return from_json::load_animation(animation, json);
	}

	auto load_animation_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<Animation>
	{
		Animation animation{};
		if (not load_animation_from_json(animation, json))
		{
			return std::nullopt;
		}

		return animation;
	}

	auto load_animation_set_from_json(AnimationSet& animation_set, const ConfigReader::json_format& json) noexcept -> bool
	{
		// "animations": "/path/to/animations.xxx"
		if (json.is_string())
		{
			const auto& config_path = json.get_ref<const std::string&>();
			SPDLOG_INFO("正在加载动画集指定配置文件[{}]...", config_path);

			// todo: 链接支持其他格式

			// "animations": "/path/to/animations.json"
			{
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("动画集指定的配置文件[{}]加载失败!", config_path);
					return false;
				}

				return from_json::load_animation_set(animation_set, *config);
			}
		}

		return from_json::load_animation_set(animation_set, json);
	}

	auto load_animation_set_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<AnimationSet>
	{
		AnimationSet animation_set{};
		if (not load_animation_set_from_json(animation_set, json))
		{
			return std::nullopt;
		}

		return animation_set;
	}
}
