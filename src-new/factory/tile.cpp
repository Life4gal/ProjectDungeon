// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/tile.hpp>

#include <component/tile.hpp>

#include <factory/detail/transform.hpp>
#include <factory/detail/render.hpp>
#include <factory/detail/collision.hpp>

#include <spdlog/spdlog.h>
#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	auto Tile::spawn(entt::registry& registry, const blueprint::Tile& tile) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// transform
		detail::attach(registry, entity, tile.position);
		// render
		detail::attach(registry, entity, tile.sprite);
		// collision & ShapeIds
		if (tile.collision.has_value())
		{
			const auto& [def, shapes] = *tile.collision;

			const auto body_id = detail::create_attach(registry, entity, def, tile.position);

			auto& [shape_ids] = registry.emplace<tile::ShapeIds>(entity);
			shape_ids.reserve(shapes.size());

			for (const auto& shape: shapes)
			{
				const auto shape_id = detail::create(body_id, shape);

				shape_ids.push_back(shape_id);
			}
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
