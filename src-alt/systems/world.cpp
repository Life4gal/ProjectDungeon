// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/world.hpp>

#include <components/world.hpp>

#include <entt/entt.hpp>

namespace pd::systems
{
	auto World::create(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		registry.ctx().emplace<world::FrameDelta>(sf::seconds(1));
		registry.ctx().emplace<world::TotalElapsed>(sf::Time::Zero);
		registry.ctx().emplace<world::PlayElapsed>(sf::Time::Zero);
	}

	auto World::destroy(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		registry.ctx().erase<world::FrameDelta>();
		registry.ctx().erase<world::TotalElapsed>();
		registry.ctx().erase<world::PlayElapsed>();
	}

	auto World::get_last_frame_delta(entt::registry& registry) noexcept -> sf::Time
	{
		using namespace components;

		const auto [frame_delta] = registry.ctx().get<const world::FrameDelta>();

		return frame_delta;
	}

	auto World::set_frame_delta(entt::registry& registry, const sf::Time this_frame_delta) noexcept -> void
	{
		using namespace components;

		auto& [frame_delta] = registry.ctx().get<world::FrameDelta>();

		frame_delta = this_frame_delta;
	}

	auto World::get_total_elapsed(entt::registry& registry) noexcept -> sf::Time
	{
		using namespace components;

		const auto [total_elapsed] = registry.ctx().get<const world::TotalElapsed>();

		return total_elapsed;
	}

	auto World::update_total_elapsed(entt::registry& registry, const sf::Time this_frame_delta) noexcept -> void
	{
		using namespace components;

		auto& [total_elapsed] = registry.ctx().get<world::TotalElapsed>();

		total_elapsed += this_frame_delta;
	}

	auto World::get_play_elapsed(entt::registry& registry) noexcept -> sf::Time
	{
		using namespace components;

		const auto [play_elapsed] = registry.ctx().get<const world::PlayElapsed>();

		return play_elapsed;
	}

	auto World::update_play_elapsed(entt::registry& registry, const sf::Time this_frame_delta) noexcept -> void
	{
		using namespace components;

		auto& [play_elapsed] = registry.ctx().get<world::PlayElapsed>();

		play_elapsed += this_frame_delta;
	}

	auto World::pause(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		registry.ctx().insert_or_assign<world::GamePaused>({});
	}

	auto World::unpause(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		registry.ctx().erase<world::GamePaused>();
	}

	auto World::is_pause(entt::registry& registry) noexcept -> bool
	{
		using namespace components;

		return registry.ctx().contains<world::GamePaused>();
	}
}
