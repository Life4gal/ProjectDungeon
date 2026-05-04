// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/player.hpp>

#include <utility/physics.hpp>

#include <component/player_controller.hpp>
#include <component/player.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

namespace pd::listener::player
{
	auto on_move_to(entt::registry& registry, const event::player::MoveTo& move_to) noexcept -> void
	{
		namespace player_controller = component::player_controller;
		namespace physics_body = component::physics_body;
		namespace transform = component::transform;

		const auto* target = registry.ctx().find<player_controller::Target>();
		if (target == nullptr)
		{
			return;
		}

		if (const auto* body_id = registry.try_get<physics_body::Id>(target->entity))
		{
			using utility::Physics;

			const auto [position, rotation] = b2Body_GetTransform(body_id->id);
			const auto new_position = b2Vec2{.x = move_to.x, .y = move_to.y};

			SPDLOG_INFO(
				"Player MoveTo: [X]={} -> {}, [Y]={} -> {}",
				Physics::from_physics(position.x),
				Physics::from_physics(new_position.x),
				Physics::from_physics(position.y),
				Physics::from_physics(new_position.y)
			);

			b2Body_SetTransform(body_id->id, new_position, rotation);
		}
		else
		{
			auto& [position] = registry.get<transform::Position>(target->entity);
			const sf::Vector2f new_position{move_to.x, move_to.y};

			SPDLOG_INFO("Player MoveTo: [X]={} -> {}, [Y]={} -> {}", position.x, new_position.x, position.y, new_position.y);

			position = new_position;
		}
	}

	auto on_translate(entt::registry& registry, const event::player::Translate& translate) noexcept -> void
	{
		namespace player_controller = component::player_controller;
		namespace physics_body = component::physics_body;
		namespace transform = component::transform;

		const auto* target = registry.ctx().find<player_controller::Target>();
		if (target == nullptr)
		{
			return;
		}

		if (const auto* body_id = registry.try_get<physics_body::Id>(target->entity))
		{
			using utility::Physics;

			const auto physics_translate_x = Physics::to_physics(translate.x);
			const auto physics_translate_y = Physics::to_physics(translate.y);

			const auto [position, rotation] = b2Body_GetTransform(body_id->id);
			const auto new_position = position + b2Vec2{.x = physics_translate_x, .y = physics_translate_y};

			SPDLOG_INFO(
				"Player Translate: [X]={} -> {}, [Y]={} -> {}",
				Physics::from_physics(position.x),
				Physics::from_physics(new_position.x),
				Physics::from_physics(position.y),
				Physics::from_physics(new_position.y)
			);

			b2Body_SetTransform(body_id->id, new_position, rotation);
		}
		else
		{
			auto& [position] = registry.get<transform::Position>(target->entity);
			const sf::Vector2f new_position{position.x + translate.x, position.y + translate.y};

			SPDLOG_INFO("Player Translate: [X]={} -> {}, [Y]={} -> {}", position.x, new_position.x, position.y, new_position.y);

			position = new_position;
		}
	}
}
