// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <config/enemy.hpp>

#include <utility/json_reader.hpp>

#include <spdlog/spdlog.h>

namespace pd::config
{
	auto load_enemy(const std::filesystem::path& file_path) noexcept -> EnemySet
	{
		EnemySet enemy_set{};

		// todo


		return enemy_set;
	}
}
