// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/collision.hpp>

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
		const blueprint::CollisionBodyDef& body_def,
		blueprint::Position position
	) noexcept -> b2BodyId;

	// ====================================
	// SHAPE -- CIRCLE
	// ====================================

	[[nodiscard]] auto create(
		b2BodyId body_id,
		const blueprint::CollisionShapeDef& shape_def,
		const blueprint::CollisionShape::circle& circle
	) noexcept -> b2ShapeId;

	// ====================================
	// SHAPE -- CAPSULE
	// ====================================

	[[nodiscard]] auto create(
		b2BodyId body_id,
		const blueprint::CollisionShapeDef& shape_def,
		const blueprint::CollisionShape::capsule& capsule
	) noexcept -> b2ShapeId;

	// ====================================
	// SHAPE -- BOX
	// ====================================

	[[nodiscard]] auto create(
		b2BodyId body_id,
		const blueprint::CollisionShapeDef& shape_def,
		const blueprint::CollisionShape::box& box
	) noexcept -> b2ShapeId;

	// ====================================
	// SHAPE -- OFFSET BOX
	// ====================================

	[[nodiscard]] auto create(
		b2BodyId body_id,
		const blueprint::CollisionShapeDef& shape_def,
		const blueprint::CollisionShape::offset_box& offset_box
	) noexcept -> b2ShapeId;

	// ====================================
	// SHAPE -- SEGMENT
	// ====================================

	[[nodiscard]] auto create(
		b2BodyId body_id,
		const blueprint::CollisionShapeDef& shape_def,
		const blueprint::CollisionShape::segment& segment
	) noexcept -> b2ShapeId;

	// ====================================
	// SHAPE
	// ====================================

	[[nodiscard]] auto create(
		b2BodyId body_id,
		const blueprint::CollisionShape& shape
	) noexcept -> b2ShapeId;

	// ====================================
	// COLLISION
	// ====================================

	auto attach(
		entt::registry& registry,
		entt::entity entity,
		const blueprint::Collision& collision,
		blueprint::Position position
	) noexcept -> void;
}
