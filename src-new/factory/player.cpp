// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/player.hpp>

#include <component/player.hpp>

#include <factory/detail/transform.hpp>
#include <factory/detail/sprite_animation.hpp>
#include <factory/detail/physics_body.hpp>
#include <factory/detail/physics_shape.hpp>
#include <factory/detail/actor.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	auto Player::spawn(entt::registry& registry, const blueprint::Player& player) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// transform
		detail::attach(registry, entity, player.transform);
		// sprite_animation
		detail::attach(registry, entity, player.animation);
		// physics_body & physics_shape
		{
			const auto body_id = detail::create_attach(registry, entity, player.transform, player.physics_body);

			const auto shape_id = detail::create(body_id, player.transform, player.physics_shape);
			registry.emplace<player::PhysicsShape>(entity, shape_id);
		}
		// actor
		detail::attach(registry, entity, player.actor, player.animation.frames.front());

		registry.emplace<player::Player>(entity);

		return entity;
	}

	auto Player::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<player::Player>();
		registry.destroy(view.begin(), view.end());
	}
}
