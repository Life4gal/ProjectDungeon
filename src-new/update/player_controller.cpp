// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/player_controller.hpp>

#include <utility/physics.hpp>

#include <component/player_controller.hpp>
#include <component/physics_body.hpp>

#include <entt/entt.hpp>
#include <SFML/System/Vector2.hpp>
#include <box2d/box2d.h>

namespace pd::update
{
	using namespace component;

	auto player_controller(entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		const auto* target = registry.ctx().find<player_controller::Target>();
		if (target == nullptr)
		{
			return;
		}

		const auto direction = [&] noexcept -> sf::Vector2f
		{
			sf::Vector2f d{0, 0};

			using enum player_controller::MovementType;
			if (const auto* horizontal = registry.ctx().find<player_controller::HorizontalMovement>())
			{
				d.x = static_cast<float>(static_cast<int>(horizontal->type));
			}
			if (const auto* vertical = registry.ctx().find<player_controller::VerticalMovement>())
			{
				d.y = static_cast<float>(static_cast<int>(vertical->type));
			}

			return d;
		}();

		if (direction == sf::Vector2f{0, 0})
		{
			return;
		}

		// 如果含有物理刚体组件
		if (const auto* body_id = registry.try_get<physics_body::Id>(target->entity))
		{
			// TODO: 系数如何计算?
			constexpr auto speed = utility::Physics::to_physics(120);
			const auto direction_normalize = direction.normalized();
			const auto mass = b2Body_GetMass(body_id->id);
			const auto force = mass * direction_normalize * speed;

			b2Body_ApplyForceToCenter(body_id->id, {.x = force.x, .y = force.y}, true);

			return;
		}

		// 其他处理方式?
	}
}
