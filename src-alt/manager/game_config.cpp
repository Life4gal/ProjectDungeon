// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <manager/game_config.hpp>

namespace pd::manager
{
	GameConfig::GameConfig() noexcept
		: window_width_(1280),
		  window_height_(720),
		  sound_volume_(100),
		  music_volume_(100),
		  max_sound_channels_(16),
		  player_name_("Unnamed") {}

	auto GameConfig::instance() noexcept -> GameConfig&
	{
		static GameConfig config;
		return config;
	}
}
