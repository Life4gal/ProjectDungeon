// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/bounding.hpp>

#include <component/bounding.hpp>

#include <factory/detail/collision.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	namespace
	{
		constexpr blueprint::CollisionBodyDef BodyDef
		{
				.type = blueprint::CollisionBodyType::STATIC,
				.fixed_rotation = true,
				.is_bullet = false,
		};
		constexpr blueprint::CollisionShapeDef ShapeDef
		{
				.material = {.friction = 0.3f, .restitution = 0},
				.density = 0,
				.category = blueprint::CollisionCategory::WALL,
				.mask = blueprint::CollisionMask::WALL,
				.is_sensor = false,
				.enable_sensor_events = false,
				.enable_contact_events = false,
		};
	}

	auto Bounding::spawn(entt::registry& registry, const blueprint::Bounding& bounding) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// collision
		{
			const auto body_id = detail::create_attach(registry, entity, BodyDef, bounding.position);

			auto& [shape_ids] = registry.emplace<bounding::ShapeIds>(entity);
			shape_ids.reserve(bounding.segments.size());

			for (const auto& segment: bounding.segments)
			{
				const auto shape_id = detail::create(body_id, ShapeDef, segment);

				shape_ids.push_back(shape_id);
			}
		}
		// bounding::Room由factory::Room附加
		// tags
		registry.emplace<tags::Bounding>(entity);

		return entity;
	}

	auto Bounding::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::Bounding>();
		registry.destroy(view.begin(), view.end());
	}
}
