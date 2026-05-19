// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/enemy.hpp>

#include <component/enemy.hpp>

#include <factory/detail/transform.hpp>
#include <factory/detail/sprite_animation.hpp>
#include <factory/detail/physics.hpp>
#include <factory/detail/actor.hpp>
#include <factory/detail/ai.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	auto Enemy::spawn(entt::registry& registry, const blueprint::Enemy& enemy) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// transform
		detail::attach(registry, entity, enemy.position);
		// sprite_animation
		detail::attach(registry, entity, enemy.animation);
		// physics
		{
			const auto body_id = detail::create_attach(registry, entity, enemy.body_desc, enemy.position);

			const auto shape_id = std::visit(detail::creator(body_id, enemy.shape_desc), enemy.shape);
			registry.emplace<enemy::PhysicsShape>(entity, shape_id);
		}
		// type
		registry.emplace<enemy::Type>(entity, static_cast<enemy::Type>(enemy.type));
		// ai
		detail::attach(registry, entity, enemy.ai);
		// actor
		detail::attach(registry, entity, enemy.actor, enemy.animation);
		// contact_damage
		registry.emplace<enemy::ContactDamage>(entity, enemy.contact_damage);

		registry.emplace<tags::Enemy>(entity);

		return entity;
	}

	auto Enemy::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::Enemy>();
		registry.destroy(view.begin(), view.end());
	}
}
