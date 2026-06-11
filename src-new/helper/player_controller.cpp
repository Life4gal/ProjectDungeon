// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/player_controller.hpp>

#include <component/player_controller.hpp>

#include <entt/entt.hpp>

namespace pd::helper
{
	using namespace component;

	auto PlayerController::left(entt::registry& registry) noexcept -> void
	{
		registry.ctx().emplace<player_controller::HorizontalMovement>(player_controller::MovementType::BACKWARD);
	}

	auto PlayerController::right(entt::registry& registry) noexcept -> void
	{
		registry.ctx().emplace<player_controller::HorizontalMovement>(player_controller::MovementType::FORWARD);
	}

	auto PlayerController::stop_horizontal(entt::registry& registry) noexcept -> void
	{
		registry.ctx().erase<player_controller::HorizontalMovement>();
	}

	auto PlayerController::up(entt::registry& registry) noexcept -> void
	{
		registry.ctx().emplace<player_controller::VerticalMovement>(player_controller::MovementType::BACKWARD);
	}

	auto PlayerController::down(entt::registry& registry) noexcept -> void
	{
		registry.ctx().emplace<player_controller::VerticalMovement>(player_controller::MovementType::FORWARD);
	}

	auto PlayerController::stop_vertical(entt::registry& registry) noexcept -> void
	{
		registry.ctx().erase<player_controller::VerticalMovement>();
	}
}
