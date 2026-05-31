// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/enemy.hpp>

#include <component/enemy.hpp>

#include <factory/detail/transform.hpp>
#include <factory/detail/render.hpp>
#include <factory/detail/collision.hpp>
#include <factory/detail/property.hpp>
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
		// render
		detail::attach(registry, entity, enemy.sprite, blueprint::RenderLayer::ENEMY);
		// collision
		detail::attach(registry, entity, enemy.collision, enemy.position);
		// property
		detail::attach(registry, entity, enemy.property);
		// ai
		detail::attach(registry, entity, enemy.ai);
		// contact_damage
		registry.emplace<enemy::ContactDamage>(entity, enemy.contact_damage);
		// type
		registry.emplace<enemy::EnemyType>(entity, enemy.type);
		// tags
		registry.emplace<tags::Enemy>(entity);

		return entity;
	}

	auto Enemy::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::Enemy>();
		registry.destroy(view.begin(), view.end());
	}
}
