// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/world.hpp>

#include <components/world.hpp>

#include <game/constants.hpp>

#include <entt/entt.hpp>

namespace pd::systems::helper
{
	auto World::create(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		// 避免可能的除0,初始delta设置为非0值
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

	auto World::window_width([[maybe_unused]] entt::registry& registry) noexcept -> int
	{
		return Constant::window_width;
	}

	auto World::window_height([[maybe_unused]] entt::registry& registry) noexcept -> int
	{
		return Constant::window_height;
	}

	auto World::window_title([[maybe_unused]] entt::registry& registry) noexcept -> std::string_view
	{
		return Constant::window_title;
	}

	auto World::frame_delta(entt::registry& registry) noexcept -> sf::Time
	{
		using namespace components;

		auto& [frame_delta] = registry.ctx().get<world::FrameDelta>();
		return frame_delta;
	}

	auto World::update_frame_delta(entt::registry& registry, const sf::Time this_frame_delta) noexcept -> void
	{
		using namespace components;

		registry.ctx().emplace<world::FrameDelta>(this_frame_delta);
	}

	auto World::total_elapsed(entt::registry& registry) noexcept -> sf::Time
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

	auto World::play_elapsed(entt::registry& registry) noexcept -> sf::Time
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

		registry.ctx().emplace<world::GamePaused>();
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
} // namespace pd::systems::helper
