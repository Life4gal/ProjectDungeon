// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/player.hpp>

#include <component/player.hpp>

#include <factory/detail/transform.hpp>
#include <factory/detail/render.hpp>
#include <factory/detail/collision.hpp>
#include <factory/detail/property.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	auto Player::spawn(entt::registry& registry, const blueprint::Player& player) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// transform
		detail::attach(registry, entity, player.position);
		// render
		detail::attach(registry, entity, player.sprite, blueprint::RenderLayer::PLAYER);
		// collision
		detail::attach(registry, entity, player.collision, player.position);
		// property
		detail::attach(registry, entity, player.property);
		// speed
		registry.emplace<player::Speed>(entity, player.speed);
		// tags
		registry.emplace<tags::Player>(entity);

		return entity;
	}

	auto Player::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::Player>();
		registry.destroy(view.begin(), view.end());
	}
}
