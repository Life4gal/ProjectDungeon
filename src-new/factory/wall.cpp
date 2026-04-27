// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/wall.hpp>

#include <component/wall.hpp>

#include <factory/detail/transform.hpp>
#include <factory/detail/sprite.hpp>
#include <factory/detail/physics_body.hpp>
#include <factory/detail/physics_shape.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	auto Wall::spawn(entt::registry& registry, const blueprint::Wall& wall) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// transform
		detail::attach(registry, entity, wall.transform);
		// sprite
		detail::attach(registry, entity, wall.sprite);
		// physics_body & physics_shape
		{
			const auto body_id = detail::create_attach(registry, entity, wall.transform, wall.physics_body);

			const auto shape_id = detail::create(body_id, wall.transform, wall.physics_shape);
			registry.emplace<wall::PhysicsShape>(entity, shape_id);
		}

		registry.emplace<wall::Wall>(entity);

		return entity;
	}

	auto Wall::destroy(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.destroy(entity);
	}

	auto Wall::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<wall::Wall>();
		registry.destroy(view.begin(), view.end());
	}
}
