// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/animation.hpp>

#include <utility/json_reader.hpp>

#include <spdlog/spdlog.h>

namespace pd::config
{
	[[nodiscard]] auto load_animation(const std::filesystem::path& path) noexcept -> AnimationSet
	{
		AnimationSet animation_set{};

		// 读取JSON配置文件
		const auto json_opt = utility::JsonReader::read_file(path);
		if (not json_opt.has_value())
		{
			return animation_set;
		}

		const auto& json = json_opt.value();

		try
		{
			if (not json.contains("animations"))
			{
				SPDLOG_ERROR("动画配置格式错误:缺少'animations'字段!");
				return animation_set;
			}

			for (const auto& animations = json["animations"];
			     const auto& [name, anim_data]: animations.items())
			{
				Animation animation{};

				if (anim_data.contains("looping"))
				{
					animation.looping = anim_data["looping"].get<bool>();
				}
				else
				{
					animation.looping = false;
				}

				if (not anim_data.contains("frames"))
				{
					SPDLOG_WARN("动画缺少帧数据,跳过!", name);
					continue;
				}

				const auto& frames = anim_data["frames"];
				animation.frames.reserve(frames.size());

				for (const auto& frame_data: frames)
				{
					AnimationFrame frame{};

					frame.texture_path = frame_data["texture_path"].get<std::string>();

					// duration_ms转换为microseconds
					const auto duration_ms = frame_data["duration_ms"].get<int>();
					frame.duration = std::chrono::milliseconds(duration_ms);

					frame.frame_x = frame_data["frame_x"].get<int>();
					frame.frame_y = frame_data["frame_y"].get<int>();
					frame.frame_width = frame_data["frame_width"].get<int>();
					frame.frame_height = frame_data["frame_height"].get<int>();

					animation.frames.emplace_back(std::move(frame));
				}

				SPDLOG_INFO("加载动画[{}]成功!共有[{}]帧", name, animation.frames.size());
				animation_set.emplace(name, std::move(animation));
			}
		}
		catch (const nlohmann::json::exception& e)
		{
			SPDLOG_ERROR("解析动画配置失败: {}", e.what());
		}

		return animation_set;
	}
}
