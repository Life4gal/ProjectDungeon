// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/physics.hpp>

#include <entt/entity/fwd.hpp>

#include <box2d/id.h>

namespace pd::factory::detail
{
	// ====================================
	// BODY
	// ====================================

	// position == 物理体位置
	[[nodiscard]] auto create_attach(
		entt::registry& registry,
		entt::entity entity,
		const blueprint::BodyDesc& body_desc,
		blueprint::Position position
	) noexcept -> b2BodyId;

	// ====================================
	// SHAPE -- CIRCLE
	// ====================================

	[[nodiscard]] auto create(
		b2BodyId body_id,
		const blueprint::ShapeDesc& shape_desc,
		const blueprint::ShapeCategory::Circle& circle
	) noexcept -> b2ShapeId;

	// ====================================
	// SHAPE -- CAPSULE
	// ====================================

	[[nodiscard]] auto create(
		b2BodyId body_id,
		const blueprint::ShapeDesc& shape_desc,
		const blueprint::ShapeCategory::Capsule& capsule
	) noexcept -> b2ShapeId;

	// ====================================
	// SHAPE -- BOX
	// ====================================

	[[nodiscard]] auto create(
		b2BodyId body_id,
		const blueprint::ShapeDesc& shape_desc,
		const blueprint::ShapeCategory::Box& box
	) noexcept -> b2ShapeId;

	// ====================================
	// SHAPE -- OFFSET BOX
	// ====================================

	[[nodiscard]] auto create(
		b2BodyId body_id,
		const blueprint::ShapeDesc& shape_desc,
		const blueprint::ShapeCategory::OffsetBox& offset_box
	) noexcept -> b2ShapeId;

	// ====================================
	// SHAPE
	// ====================================

	[[nodiscard]] constexpr auto creator(const b2BodyId body_id, const blueprint::ShapeDesc& shape_desc) noexcept -> auto
	{
		return
				[body_id, &shape_desc](const auto category) noexcept -> b2ShapeId //
					requires requires { create(body_id, shape_desc, category); }
		{
			return create(body_id, shape_desc, category);
		};
	}
}
