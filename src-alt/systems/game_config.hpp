// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string_view>

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

namespace pd::systems
{
	class GameConfig final
	{
	public:
		// ============================
		// Game
		// ============================

		[[nodiscard]] static auto get_window_size(entt::registry& registry) noexcept -> sf::Vector2u;

		[[nodiscard]] static auto get_absolute_time(entt::registry& registry) noexcept -> sf::Time;

		// ============================
		// GameConfig
		// ============================

		[[nodiscard]] static auto get_sound_volume(entt::registry& registry) noexcept -> float;

		[[nodiscard]] static auto get_music_volume(entt::registry& registry) noexcept -> float;

		[[nodiscard]] static auto get_max_sound_channels(entt::registry& registry) noexcept -> std::uint32_t;

		[[nodiscard]] static auto get_player_name(entt::registry& registry) noexcept -> std::string_view;
	};
}
