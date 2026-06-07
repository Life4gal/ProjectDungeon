// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <assembly/ai.hpp>

#include <component/ai.hpp>

#include <prometheus/functional/functor.hpp>
#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>

namespace pd::assembly
{
	using namespace component;

	auto Ai::make_move_behavior(entt::registry& registry, const entt::entity entity, const blueprint::MoveBehavior& move_behavior) noexcept -> void
	{
		const auto visitor = prometheus::functional::overloaded
		{
				[&](const blueprint::MoveBehavior::Stationary&) noexcept -> void
				{
					//
				},
				[&](const blueprint::MoveBehavior::Wander& wander) noexcept -> void
				{
					using namespace move_behavior::wander;

					registry.emplace<Config>(
						entity,
						Config
						{
								.speed = wander.speed,
								.next_turn_min_time = wander.next_turn_min_time,
								.next_turn_max_timer = wander.next_turn_max_timer
						}
					);
					registry.emplace<Direction>(entity, sf::degrees(0));
					registry.emplace<DirectionTimer>(entity, sf::Time::Zero);
				},
				[&](const blueprint::MoveBehavior::Patrol& patrol) noexcept -> void
				{
					std::ignore = patrol;
					PROMETHEUS_PLATFORM_UNREACHABLE();
				},
				[&](const blueprint::MoveBehavior::Chase& chase) noexcept -> void
				{
					using namespace move_behavior::chase;

					registry.emplace<Config>(entity, Config{.speed = chase.speed});
				},
				[&](const blueprint::MoveBehavior::Jump& jump) noexcept -> void
				{
					std::ignore = jump;
					PROMETHEUS_PLATFORM_UNREACHABLE();
				},
				[&](const blueprint::MoveBehavior::ChaseJump& chase_jump) noexcept -> void
				{
					using namespace move_behavior::chase_jump;

					registry.emplace<Config>(
						entity,
						Config
						{
								.speed = chase_jump.speed,
								.duration = chase_jump.duration,
								.next_jump_min_time = chase_jump.next_jump_min_time,
								.next_jump_max_time = chase_jump.next_jump_max_time
						}
					);
					registry.emplace<State>(entity, State::IDLE);
					registry.emplace<AirTimer>(entity, sf::Time::Zero);
				},
				[&](const blueprint::MoveBehavior::Teleport& teleport) noexcept -> void
				{
					std::ignore = teleport;
					PROMETHEUS_PLATFORM_UNREACHABLE();
				}
		};

		std::visit(visitor, move_behavior.behavior);
	}

	auto Ai::make(entt::registry& registry, const entt::entity entity, const blueprint::Ai& ai) noexcept -> void
	{
		// ==============================
		// MOVE BEHAVIOR
		// ==============================

		make_move_behavior(registry, entity, ai.move_behavior);

		// ==============================
		// 
		// ==============================
	}
}
