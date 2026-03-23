// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string_view>

#include <entt/fwd.hpp>

namespace pd::systems
{
	class GameConfig final
	{
	public:
		[[nodiscard]] static auto get_sound_volume(entt::registry& registry) noexcept -> float;

		[[nodiscard]] static auto get_music_volume(entt::registry& registry) noexcept -> float;

		[[nodiscard]] static auto get_max_sound_channels(entt::registry& registry) noexcept -> std::uint32_t;

		[[nodiscard]] static auto get_player_name(entt::registry& registry) noexcept -> std::string_view;
	};
}
