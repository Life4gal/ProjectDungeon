// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/player_controller.hpp>

#include <algorithm>

#include <utility/physics.hpp>

#include <component/player_controller.hpp>
#include <component/actor.hpp>
#include <component/physics_body.hpp>

#include <entt/entt.hpp>
#include <SFML/System/Vector2.hpp>
#include <box2d/box2d.h>

namespace pd::update
{
	using namespace component;

	auto player_controller(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		const auto delta_seconds = delta.asSeconds();

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

		// 如果含有物理刚体组件
		if (const auto* body_id = registry.try_get<physics_body::Id>(target->entity))
		{
			if (direction == sf::Vector2f{0, 0})
			{
				// 没有方向输入时停止
				b2Body_SetLinearVelocity(body_id->id, {.x = 0, .y = 0});
			}
			else
			{
				constexpr auto responsiveness = 10.f;
				const auto pixels_max_speed = [&] noexcept -> float
				{
					if (const auto* speed = registry.try_get<actor::Speed>(target->entity);
						speed)
					{
						return speed->speed;
					}

					return 120.f;
				}();
				const auto physics_max_speed = utility::Physics::to_physics(pixels_max_speed);

				const auto direction_normalize = direction.normalized();
				const auto max_velocity = direction_normalize * physics_max_speed;
				const auto current_velocity = b2Body_GetLinearVelocity(body_id->id);

				const auto t = std::ranges::clamp(delta_seconds * responsiveness, 0.f, 1.f);
				const auto velocity = current_velocity + (b2Vec2{.x = max_velocity.x, .y = max_velocity.y} - current_velocity) * t;

				b2Body_SetLinearVelocity(body_id->id, velocity);
			}

			return;
		}

		// 其他处理方式?
	}
}
