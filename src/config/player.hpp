// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

#include <utility/string_hash.hpp>

namespace pd::config
{
	// 玩家的数据
	class Player final
	{
	public:
		// 尺寸(比例)
		float scale_x;
		float scale_y;

		// 动画名称
		std::string animation_name;

		// 生命值
		float health;
		// 魔法值
		float mana;
		// 移动速度
		float speed;

		// 武器的名称列表
		std::vector<std::string> weapons;
	};

	// 玩家集
	// 玩家名称<->玩家数据
	class PlayerSet final : public std::unordered_map<
				std::string,
				Player,
				utility::StringHash,
				std::ranges::equal_to
			> {};

	// 从文件中读取玩家数据
	[[nodiscard]] auto load_player(const std::filesystem::path& path) noexcept -> PlayerSet;
}
