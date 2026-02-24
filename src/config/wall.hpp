// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>

#include <utility/string_hash.hpp>

namespace pd::config
{
	// 墙壁配置数据
	class Wall final
	{
	public:
		// 纹理路径
		std::string texture_path;
		// 纹理位置
		int texture_x;
		int texture_y;
		// 纹理大小
		int texture_width;
		int texture_height;
		// 碰撞体宽度(像素)
		float collision_width;
		// 碰撞体高度(像素)
		float collision_height;
		// 生命值(负数表示不可破坏)
		int health;
	};

	// 墙壁集合
	// 墙壁ID -> 墙壁配置
	class WallSet final : public std::unordered_map<
				std::string,
				Wall,
				utility::StringHash,
				std::ranges::equal_to
			>
	{
	public:
		using unordered_map::unordered_map;
	};

	// 加载墙壁配置
	[[nodiscard]] auto load_wall(const std::filesystem::path& path) noexcept -> WallSet;
} // namespace pd::config
