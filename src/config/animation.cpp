// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/animation.hpp>

#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace pd::config
{
	namespace
	{
		namespace from_json
		{
			[[nodiscard]] auto load_animation_frame(AnimationFrame& frame, const ConfigReader::json_format& json) noexcept -> bool
			{
				try
				{
					frame.texture_path = json["texture_path"].get<std::string>();
					frame.texture_x = json["texture_x"].get<int>();
					frame.texture_y = json["texture_y"].get<int>();
					frame.texture_width = json["texture_width"].get<int>();
					frame.texture_height = json["texture_height"].get<int>();
					frame.duration_ms = json["duration_ms"].get<int>();

					return true;
				}
				catch (const nlohmann::json::exception& e)
				{
					SPDLOG_ERROR("解析AnimationFrame配置失败: {}", e.what());
					return false;
				}
			}

			[[nodiscard]] auto load_animation(Animation& animation, const ConfigReader::json_format& json) noexcept -> bool
			{
				const auto frames_it = json.find("frames");
				if (frames_it == json.end())
				{
					SPDLOG_ERROR("解析Animation配置失败: 缺少'frames'字段!\n{}", json.dump(4));
					return false;
				}

				const auto& frames = frames_it.value();
				animation.frames.reserve(json.size());

				try
				{
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

					animation.looping = json.value("looping", false);

					return true;
				}
				catch (const nlohmann::json::exception& e)
				{
					SPDLOG_ERROR("解析Animation配置失败: {}", e.what());
					return false;
				}
			}

			[[nodiscard]] auto load_animation_set(AnimationSet& animation_set, const ConfigReader::json_format& json) noexcept -> bool
			{
				SPDLOG_INFO("===========================================");
				SPDLOG_INFO("=============== ->ANIMATION-SET-> ===============");
				SPDLOG_INFO("===========================================");

				animation_set.reserve(json.size());

				for (const auto& [id, data]: json.items())
				{
					SPDLOG_INFO("加载动画[{}]...", id);

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

				SPDLOG_INFO("===========================================");
				SPDLOG_INFO("=============== <-ANIMATION-SET<- ===============");
				SPDLOG_INFO("===========================================");

				return true;
			}
		} // namespace from_json

		//
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
		if (json.is_string())
		{
			// todo: 链接支持其他格式

			// json
			{
				// "animations": "/path/to/animations.json"
				const auto& config_path = json.get_ref<const std::string&>();
				const auto config = ConfigReader::read_json(config_path);

				if (not config.has_value())
				{
					SPDLOG_ERROR("AnimationSet的配置文件[{}]加载失败!", config_path);
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
