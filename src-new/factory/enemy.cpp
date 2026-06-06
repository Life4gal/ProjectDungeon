// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/enemy.hpp>

#include <component/enemy.hpp>

#include <assembly/transform.hpp>
#include <assembly/render.hpp>
#include <assembly/collision.hpp>
#include <assembly/property.hpp>
#include <assembly/ai.hpp>
#include <assembly/name.hpp>

#include <spdlog/spdlog.h>
#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	auto Enemy::spawn(entt::registry& registry, const blueprint::Enemy& enemy) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// transform
		assembly::Transform::make(registry, entity, enemy.position);
		// render
		assembly::Render::make(registry, entity, enemy.sprite, blueprint::RenderLayer::ENEMY);
		// collision
		assembly::Collision::make(registry, entity, enemy.collision, enemy.position);
		// property
		assembly::Property::make(registry, entity, enemy.property);
		// ai
		assembly::Ai::make(registry, entity, enemy.ai);
		// name
		assembly::Name::make(registry, entity, enemy.name);
		// contact_damage
		registry.emplace<enemy::ContactDamage>(entity, enemy.contact_damage);
		// type
		registry.emplace<enemy::EnemyType>(entity, enemy.type);
		// tags
		registry.emplace<tags::Enemy>(entity);

		// 新生成
		registry.emplace<state::EntityNew>(entity);

		return entity;
	}

	auto Enemy::destroy_all(entt::registry& registry) noexcept -> void
	{
		SPDLOG_INFO("正在销毁所有敌人...");

		const auto view = registry.view<tags::Enemy>();
		registry.destroy(view.begin(), view.end());

		SPDLOG_INFO("已销毁{}个敌人", view.size());
	}
}
