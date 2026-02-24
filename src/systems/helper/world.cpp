// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/world.hpp>

#include <components/world.hpp>

#include <entt/entt.hpp>

namespace pd::systems::helper
{
	auto World::frame_delta(entt::registry& registry) noexcept -> sf::Time
	{
		using namespace components::world;

		auto& [frame_delta] = registry.ctx().get<FrameDelta>();
		return frame_delta;
	}

	auto World::update_frame_delta(entt::registry& registry, const sf::Time this_frame_delta) noexcept -> void
	{
		using namespace components::world;

		registry.ctx().emplace<FrameDelta>(this_frame_delta);
	}

	auto World::total_elapsed(entt::registry& registry) noexcept -> sf::Time
	{
		using namespace components::world;

		const auto [total_elapsed] = registry.ctx().get<const TotalElapsed>();
		return total_elapsed;
	}

	auto World::update_total_elapsed(entt::registry& registry, const sf::Time this_frame_delta) noexcept -> void
	{
		using namespace components::world;

		auto& [total_elapsed] = registry.ctx().get<TotalElapsed>();
		total_elapsed += this_frame_delta;
	}

	auto World::play_elapsed(entt::registry& registry) noexcept -> sf::Time
	{
		using namespace components::world;

		const auto [play_elapsed] = registry.ctx().get<const PlayElapsed>();
		return play_elapsed;
	}

	auto World::update_play_elapsed(entt::registry& registry, const sf::Time this_frame_delta) noexcept -> void
	{
		using namespace components::world;

		auto& [play_elapsed] = registry.ctx().get<PlayElapsed>();
		play_elapsed += this_frame_delta;
	}

	auto World::pause(entt::registry& registry) noexcept -> void
	{
		using namespace components::world;

		registry.ctx().emplace<GamePaused>();
	}

	auto World::unpause(entt::registry& registry) noexcept -> void
	{
		using namespace components::world;

		registry.ctx().erase<GamePaused>();
	}

	auto World::is_pause(entt::registry& registry) noexcept -> bool
	{
		using namespace components::world;

		return registry.ctx().contains<GamePaused>();
	}
} // namespace pd::systems::helper
