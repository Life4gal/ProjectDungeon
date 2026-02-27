// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <config/level.hpp>

namespace pd::config
{
	class Dungeon final
	{
	public:
		// 地下城名称
		std::string name;

		// 关卡集
		LevelSet level_set;
		// 关卡顺序列表(关卡ID)
		std::vector<std::string> level_order;
	};

	[[nodiscard]] auto load_dungeon_from_json(Dungeon& dungeon, const ConfigReader::json_format& json) noexcept -> bool;
	[[nodiscard]] auto load_dungeon_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<Dungeon>;
}
