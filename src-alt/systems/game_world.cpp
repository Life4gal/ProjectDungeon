// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/game_world.hpp>

#include <components/game_world.hpp>

#include <entt/entt.hpp>

namespace pd::systems
{
	auto GameWorld::create(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		registry.ctx().emplace<game_world::FrameDelta>(sf::seconds(1));
		registry.ctx().emplace<game_world::TotalElapsed>(sf::Time::Zero);
		registry.ctx().emplace<game_world::PlayElapsed>(sf::Time::Zero);
	}

	auto GameWorld::destroy(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		registry.ctx().erase<game_world::FrameDelta>();
		registry.ctx().erase<game_world::TotalElapsed>();
		registry.ctx().erase<game_world::PlayElapsed>();
	}

	auto GameWorld::get_last_frame_delta(entt::registry& registry) noexcept -> sf::Time
	{
		using namespace components;

		const auto [frame_delta] = registry.ctx().get<const game_world::FrameDelta>();

		return frame_delta;
	}

	auto GameWorld::set_frame_delta(entt::registry& registry, const sf::Time this_frame_delta) noexcept -> void
	{
		using namespace components;

		auto& [frame_delta] = registry.ctx().get<game_world::FrameDelta>();

		frame_delta = this_frame_delta;
	}

	auto GameWorld::get_total_elapsed(entt::registry& registry) noexcept -> sf::Time
	{
		using namespace components;

		const auto [total_elapsed] = registry.ctx().get<const game_world::TotalElapsed>();

		return total_elapsed;
	}

	auto GameWorld::update_total_elapsed(entt::registry& registry, const sf::Time this_frame_delta) noexcept -> void
	{
		using namespace components;

		auto& [total_elapsed] = registry.ctx().get<game_world::TotalElapsed>();

		total_elapsed += this_frame_delta;
	}

	auto GameWorld::get_play_elapsed(entt::registry& registry) noexcept -> sf::Time
	{
		using namespace components;

		const auto [play_elapsed] = registry.ctx().get<const game_world::PlayElapsed>();

		return play_elapsed;
	}

	auto GameWorld::update_play_elapsed(entt::registry& registry, const sf::Time this_frame_delta) noexcept -> void
	{
		using namespace components;

		auto& [play_elapsed] = registry.ctx().get<game_world::PlayElapsed>();

		play_elapsed += this_frame_delta;
	}

	auto GameWorld::pause(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		registry.ctx().insert_or_assign<game_world::GamePaused>({});
	}

	auto GameWorld::unpause(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		registry.ctx().erase<game_world::GamePaused>();
	}

	auto GameWorld::is_pause(entt::registry& registry) noexcept -> bool
	{
		using namespace components;

		return registry.ctx().contains<game_world::GamePaused>();
	}
}
