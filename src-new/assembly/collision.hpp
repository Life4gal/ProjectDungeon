// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/collision.hpp>

#include <entt/entity/fwd.hpp>

#include <box2d/id.h>

namespace pd::assembly
{
	class Collision final
	{
	public:
		// ====================================
		// BODY
		// ====================================

		// position == 物理体位置
		[[nodiscard]] static auto make_body(
			entt::registry& registry,
			entt::entity entity,
			const blueprint::CollisionBodyDef& body_def,
			blueprint::Position position
		) noexcept -> b2BodyId;

		// ====================================
		// SHAPE -- CIRCLE
		// ====================================

		[[nodiscard]] static auto make_shape(
			b2BodyId body_id,
			const blueprint::CollisionShapeDef& shape_def,
			const blueprint::CollisionShape::Circle& circle
		) noexcept -> b2ShapeId;

		// ====================================
		// SHAPE -- CAPSULE
		// ====================================

		[[nodiscard]] static auto make_shape(
			b2BodyId body_id,
			const blueprint::CollisionShapeDef& shape_def,
			const blueprint::CollisionShape::Capsule& capsule
		) noexcept -> b2ShapeId;

		// ====================================
		// SHAPE -- BOX
		// ====================================

		[[nodiscard]] static auto make_shape(
			b2BodyId body_id,
			const blueprint::CollisionShapeDef& shape_def,
			const blueprint::CollisionShape::Box& box
		) noexcept -> b2ShapeId;

		// ====================================
		// SHAPE -- OFFSET BOX
		// ====================================

		[[nodiscard]] static auto make_shape(
			b2BodyId body_id,
			const blueprint::CollisionShapeDef& shape_def,
			const blueprint::CollisionShape::OffsetBox& offset_box
		) noexcept -> b2ShapeId;

		// ====================================
		// SHAPE -- SEGMENT
		// ====================================

		[[nodiscard]] static auto make_shape(
			b2BodyId body_id,
			const blueprint::CollisionShapeDef& shape_def,
			const blueprint::CollisionShape::Segment& segment
		) noexcept -> b2ShapeId;

		// ====================================
		// SHAPE
		// ====================================

		[[nodiscard]] static auto make_shape(
			b2BodyId body_id,
			const blueprint::CollisionShape& shape
		) noexcept -> b2ShapeId;

		// ====================================
		// COLLISION
		// ====================================

		static auto make(
			entt::registry& registry,
			entt::entity entity,
			const blueprint::Collision& collision,
			blueprint::Position position
		) noexcept -> void;
	};
}
