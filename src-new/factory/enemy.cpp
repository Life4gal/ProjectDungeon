// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/enemy.hpp>

#include <component/enemy.hpp>
#include <component/ai.hpp>

#include <factory/detail/position.hpp>
#include <factory/detail/sprite_animation.hpp>
#include <factory/detail/physics_body.hpp>
#include <factory/detail/physics_shape.hpp>
#include <factory/detail/actor.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	auto Enemy::spawn(entt::registry& registry, const blueprint::Enemy& enemy) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// position
		detail::attach(registry, entity, enemy.position);
		// sprite_animation
		detail::attach(registry, entity, enemy.animation);
		// physics_body & physics_shape
		{
			const auto body_id = detail::create_attach(registry, entity, enemy.physics_body, enemy.position);

			const auto creator = [&](const auto& shape) noexcept -> b2ShapeId
			{
				return detail::create(body_id, shape, enemy.animation);
			};
			const auto shape_id = std::visit(creator, enemy.physics_shape);
			registry.emplace<enemy::PhysicsShape>(entity, shape_id);
		}
		// actor
		detail::attach(registry, entity, enemy.actor, enemy.animation);
		// contact_damage
		registry.emplace<enemy::ContactDamage>(entity, enemy.contact_damage);
		// type
		registry.emplace<enemy::Type>(entity, static_cast<enemy::Type>(enemy.type));
		// ai
		{
			switch (enemy.ai.move_behavior)
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
			}
		}

		registry.emplace<tags::Enemy>(entity);

		return entity;
	}

	auto Enemy::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::Enemy>();
		registry.destroy(view.begin(), view.end());
	}
}
