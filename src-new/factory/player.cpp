// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/player.hpp>

#include <component/player.hpp>

#include <assembly/transform.hpp>
#include <assembly/render.hpp>
#include <assembly/collision.hpp>
#include <assembly/property.hpp>
#include <assembly/name.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	auto Player::spawn(entt::registry& registry, const blueprint::Player& player) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// transform
		assembly::Transform::make(registry, entity, player.position);
		// render
		assembly::Render::make(registry, entity, player.sprite);
		// collision
		assembly::Collision::make(registry, entity, player.collision, player.position);
		// property
		assembly::Property::make(registry, entity, player.property);
		// name
		assembly::Name::make(registry, entity, player.name);
		// speed
		registry.emplace<player::Speed>(entity, player.speed);
		// tags
		registry.emplace<tags::Player>(entity);

		// 新生成
		registry.emplace<state::EntityNew>(entity);

		return entity;
	}

	auto Player::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::Player>();
		registry.destroy(view.begin(), view.end());
	}
}
