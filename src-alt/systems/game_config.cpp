// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/game_config.hpp>

#include <components/game.hpp>

#include <game/game.hpp>

#include <prometheus/platform/os.hpp>

#include <entt/entt.hpp>

namespace pd::systems
{
	auto GameConfig::get_window_size(entt::registry& registry) noexcept -> sf::Vector2u
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::Game>());

		const auto& [game] = registry.ctx().get<const components::Game>();
		const auto window_size = game.get().window_size();

		return window_size;
	}

	auto GameConfig::get_absolute_time(entt::registry& registry) noexcept -> sf::Time
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::Game>());

		const auto& [game] = registry.ctx().get<const components::Game>();
		const auto absolute_time = game.get().time();

		return absolute_time;
	}

	auto GameConfig::get_sound_volume(entt::registry& registry) noexcept -> float
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::Game>());

		const auto& [game] = registry.ctx().get<const components::Game>();
		const auto& config = game.get().game_config();

		return config.sound_volume;
	}

	auto GameConfig::get_music_volume(entt::registry& registry) noexcept -> float
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::Game>());

		const auto& [game] = registry.ctx().get<const components::Game>();
		const auto& config = game.get().game_config();

		return config.music_volume;
	}

	auto GameConfig::get_max_sound_channels(entt::registry& registry) noexcept -> std::uint32_t
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::Game>());

		const auto& [game] = registry.ctx().get<const components::Game>();
		const auto& config = game.get().game_config();

		return config.max_sound_channels;
	}

	auto GameConfig::get_player_name(entt::registry& registry) noexcept -> std::string_view
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::Game>());

		const auto& [game] = registry.ctx().get<const components::Game>();
		const auto& config = game.get().game_config();

		return config.player_name;
	}
}
