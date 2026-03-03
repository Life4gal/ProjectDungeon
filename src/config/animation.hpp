// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <optional>

#include <config/config_reader.hpp>
#include <config/set.hpp>

namespace pd::config
{
	// JSON:
	// { "texture_path": "path/to/texture.png", "texture_x": 0, "texture_y": 0, "duration_ms": 100 }
	class AnimationFrame final
	{
	public:
		// 纹理路径
		std::string texture_path;
		// 纹理位置
		int texture_x;
		int texture_y;
		// 持续时间(毫秒)
		int duration_ms;
	};

	// JSON:
	// { "frames": [ { AnimationFrame }, { AnimationFrame}, ... ], "texture_width": 16, "texture_height": 16, "looping": true }
	class Animation final
	{
	public:
		// 动画帧
		std::vector<AnimationFrame> frames;

		// 纹理大小
		int texture_width;
		int texture_height;
		// 原点
		// 如果未指定则为(width/2, height/2)
		int origin_x;
		int origin_y;

		// 是否循环
		bool looping;
	};

	// 动画集
	// 动画ID -> 动画配置
	// JSON:
	// { "animations": "/path/to/animations.json" }
	// { "animations": { "animation-id-1": { Animation }, "animation-id-2": { Animation }, ... } }
	// 不允许:
	// "animations": { "animation-id-1": { Animation }, "animation-id-2": "/path/to/animation-id-2-config.json", ... }
	class AnimationSet final : public Set<Animation>
	{
	public:
		using Set::Set;
	};

	// ===========================
	// JSON
	// ===========================

	[[nodiscard]] auto load_animation_frame_from_json(AnimationFrame& frame, const ConfigReader::json_format& json) noexcept -> bool;
	[[nodiscard]] auto load_animation_frame_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<AnimationFrame>;

	[[nodiscard]] auto load_animation_from_json(Animation& animation, const ConfigReader::json_format& json) noexcept -> bool;
	[[nodiscard]] auto load_animation_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<Animation>;

	[[nodiscard]] auto load_animation_set_from_json(AnimationSet& animation_set, const ConfigReader::json_format& json) noexcept -> bool;
	[[nodiscard]] auto load_animation_set_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<AnimationSet>;

	// ===========================
	// 
	// ===========================
}
