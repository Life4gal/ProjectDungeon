// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string_view>

#include <entt/fwd.hpp>

#include <SFML/System/Time.hpp>

namespace pd::systems::helper
{
	class World final
	{
	public:
		static auto create(entt::registry& registry) noexcept -> void;

		static auto destroy(entt::registry& registry) noexcept -> void;

		// =============================================
		// WINDOW
		// =============================================

		[[nodiscard]] static auto window_width(entt::registry& registry) noexcept -> int;

		[[nodiscard]] static auto window_height(entt::registry& registry) noexcept -> int;

		[[nodiscard]] static auto window_title(entt::registry& registry) noexcept -> std::string_view;

		// =============================================
		// GAME
		// =============================================

		[[nodiscard]] static auto frame_delta(entt::registry& registry) noexcept -> sf::Time;

		static auto update_frame_delta(entt::registry& registry, sf::Time this_frame_delta) noexcept -> void;

		[[nodiscard]] static auto total_elapsed(entt::registry& registry) noexcept -> sf::Time;

		static auto update_total_elapsed(entt::registry& registry, sf::Time this_frame_delta) noexcept -> void;

		[[nodiscard]] static auto play_elapsed(entt::registry& registry) noexcept -> sf::Time;

		static auto update_play_elapsed(entt::registry& registry, sf::Time this_frame_delta) noexcept -> void;

		static auto pause(entt::registry& registry) noexcept -> void;

		static auto unpause(entt::registry& registry) noexcept -> void;

		[[nodiscard]] static auto is_pause(entt::registry& registry) noexcept -> bool;
	};
}
