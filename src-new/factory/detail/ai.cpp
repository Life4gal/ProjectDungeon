// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/ai.hpp>

#include <component/ai.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>

namespace pd::factory::detail
{
	using namespace component;

	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::Ai& ai) noexcept -> void
	{
		switch (ai.move_behavior)
		{
			case blueprint::MoveBehavior::STATIONARY:
			{
				// 静止的敌人无需任何额外组件
				break;
			}
			case blueprint::MoveBehavior::WANDER:
			{
				registry.emplace<ai::wander::Direction>(entity, sf::degrees(0));
				registry.emplace<ai::wander::DirectionTimer>(entity, sf::Time::Zero);

				break;
			}
			case blueprint::MoveBehavior::JUMP:
			{
				registry.emplace<ai::jump::State>(entity, ai::jump::State::IDLE);
				registry.emplace<ai::jump::AirTimer>(entity, sf::Time::Zero);

				break;
			}
			case blueprint::MoveBehavior::CHASE:
			{
				registry.emplace<ai::chase::Placeholder>(entity);

				break;
			}
			case blueprint::MoveBehavior::TELEPORT:
			{
				// TODO
				break;
			}
			default: // NOLINT(clang-diagnostic-covered-switch-default)
			{
				PROMETHEUS_PLATFORM_UNREACHABLE();
			}
		}
	}
}
