// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/player_controller.hpp>

#include <component/player_controller.hpp>
#include <component/player.hpp>

#include <helper/physics.hpp>
#include <helper/transform.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::helper
{
	using namespace component;

	auto PlayerController::online(entt::registry& registry) noexcept -> bool
	{
		return registry.ctx().find<player_controller::Target>() != nullptr;
	}

	auto PlayerController::target(entt::registry& registry) noexcept -> entt::entity
	{
		const auto* target = registry.ctx().find<const player_controller::Target>();

		if (target == nullptr)
		{
			return entt::null;
		}

		return target->entity;
	}

	auto PlayerController::set_target(entt::registry& registry, const entt::entity new_target) noexcept -> void
	{
		registry.ctx().insert_or_assign(player_controller::Target{.entity = new_target});
	}

	auto PlayerController::position(entt::registry& registry) noexcept -> sf::Vector2f
	{
		const auto e = target(registry);
		if (e == entt::null)
		{
			return {0, 0};
		}

		return Transform::get_position(registry, e);
	}

	auto PlayerController::screen_position(entt::registry& registry) noexcept -> sf::Vector2i
	{
		const auto e = target(registry);
		if (e == entt::null)
		{
			return {0, 0};
		}

		return Transform::get_screen_position(registry, e);
	}

	auto PlayerController::move_to(entt::registry& registry, const sf::Vector2f new_position) noexcept -> void
	{
		const auto e = target(registry);
		if (e == entt::null)
		{
			return;
		}

		const auto old_position = Transform::get_position(registry, e);
		SPDLOG_INFO(
			"玩家位置移动: [X]={} -> {}({}), [Y]={} -> {}({})",
			old_position.x,
			new_position.x,
			new_position.x - old_position.x,
			old_position.y,
			new_position.y,
			new_position.y - old_position.y
		);

		if (registry.all_of<physics::BodyId>(e))
		{
			Physics::set_pixel_position(registry, e, new_position);
		}
		else
		{
			Transform::set_position(registry, e, new_position);
		}
	}

	auto PlayerController::move_to_screen(entt::registry& registry, const sf::Vector2i new_position) noexcept -> void
	{
		const auto e = target(registry);
		if (e == entt::null)
		{
			return;
		}

		const auto old_position = Transform::get_screen_position(registry, e);
		SPDLOG_INFO(
			"玩家(屏幕)位置移动: [X]={} -> {}({}), [Y]={} -> {}({})",
			old_position.x,
			new_position.x,
			new_position.x - old_position.x,
			old_position.y,
			new_position.y,
			new_position.y - old_position.y
		);

		if (registry.all_of<physics::BodyId>(e))
		{
			Physics::set_screen_position(registry, e, new_position);
		}
		else
		{
			Transform::set_screen_position(registry, e, new_position);
		}
	}

	auto PlayerController::translate(entt::registry& registry, const sf::Vector2f distance) noexcept -> void
	{
		const auto e = target(registry);
		if (e == entt::null)
		{
			return;
		}

		const auto old_position = Transform::get_position(registry, e);
		const auto new_position = old_position + distance;
		SPDLOG_INFO(
			"玩家位置移动: [X]={} -> {}({}), [Y]={} -> {}({})",
			old_position.x,
			new_position.x,
			new_position.x - old_position.x,
			old_position.y,
			new_position.y,
			new_position.y - old_position.y
		);

		if (registry.all_of<physics::BodyId>(e))
		{
			// Physics::translate_pixel(registry, e, distance);
			Physics::set_pixel_position(registry, e, new_position);
		}
		// TODO: PlayerController::translate只会设置物理刚体的位置(如果当前控制的实体存在物理刚体组件)
		//  但是Transform的位置需要在下一帧的sync_physics_transform中同步,而render::camera在计算视野内的实体时使用的是Transform的位置
		//  这意味着如果我们要么延迟一帧更新Camera,要么手动进行同步
		// else
		{
			// Transform::translate(registry, e, distance);
			Transform::set_position(registry, e, new_position);
		}
	}

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
