// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/tile.hpp>

#include <component/tile.hpp>

#include <assembly/transform.hpp>
#include <assembly/render.hpp>
#include <assembly/collision.hpp>

#include <spdlog/spdlog.h>
#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	auto Tile::spawn(entt::registry& registry, const blueprint::Tile& tile) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// transform
		assembly::Transform::make(registry, entity, tile.position);
		// render
		assembly::Render::make(registry, entity, tile.sprite);
		// collision & ShapeIds
		if (tile.collision.has_value())
		{
			assembly::Collision::make(registry, entity, *tile.collision, tile.position);
		}
		// tags
		registry.emplace<tags::Tile>(entity);

		// 新生成
		registry.emplace<state::EntityNew>(entity);

		return entity;
	}

	auto Tile::destroy_all(entt::registry& registry) noexcept -> void
	{
		SPDLOG_INFO("正在销毁所有瓦片...");

		const auto view = registry.view<tags::Tile>();
		registry.destroy(view.begin(), view.end());

		SPDLOG_INFO("已销毁{}个瓦片", view.size());
	}
}
