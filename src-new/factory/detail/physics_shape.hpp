// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/physics_shape.hpp>
#include <blueprint/detail/sprite.hpp>
#include <blueprint/detail/sprite_animation.hpp>

#include <box2d/id.h>

namespace pd::factory::detail
{
	// ====================================
	// CIRCLE
	// ====================================

	// scale == 目标实体的缩放
	// radius == circle.radius * scale
	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::PhysicsShapeCircle& circle, blueprint::Scale scale) noexcept -> b2ShapeId;

	// scale == sprite.scale
	// radius == circle.radius * scale
	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::PhysicsShapeCircle& circle, const blueprint::Sprite& sprite) noexcept -> b2ShapeId;

	// scale == sprite_animation.scale
	// radius == circle.radius * scale
	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::PhysicsShapeCircle& circle, const blueprint::SpriteAnimation& sprite_animation) noexcept -> b2ShapeId;

	// ====================================
	// CAPSULE
	// ====================================

	// scale == 目标实体的缩放
	// center1 == capsule.center1 * scale
	// center2 == capsule.center2 * scale
	// radius == capsule.radius * scale
	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::PhysicsShapeCapsule& capsule, blueprint::Scale scale) noexcept -> b2ShapeId;

	// scale == sprite.scale
	// center1 == capsule.center1 * scale
	// center2 == capsule.center2 * scale
	// radius == capsule.radius * scale
	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::PhysicsShapeCapsule& capsule, const blueprint::Sprite& sprite) noexcept -> b2ShapeId;

	// scale == sprite_animation.scale
	// center1 == capsule.center1 * scale
	// center2 == capsule.center2 * scale
	// radius == capsule.radius * scale
	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::PhysicsShapeCapsule& capsule, const blueprint::SpriteAnimation& sprite_animation) noexcept -> b2ShapeId;

	// ====================================
	// BOX
	// ====================================

	// scale == 目标实体的缩放
	// size = box.size * scale
	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::PhysicsShapeBox& box, blueprint::Scale scale) noexcept -> b2ShapeId;

	// scale == sprite.scale
	// size = box.size * scale
	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::PhysicsShapeBox& box, const blueprint::Sprite& sprite) noexcept -> b2ShapeId;

	// scale == sprite_animation.scale
	// size = box.size * scale
	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::PhysicsShapeBox& box, const blueprint::SpriteAnimation& sprite_animation) noexcept -> b2ShapeId;

	// ====================================
	// OFFSET BOX
	// ====================================

	// scale == 目标实体的缩放
	// rotation == 目标实体的旋转
	// size = offset_box.size * scale
	// center = offset_box.offset*scale
	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::PhysicsShapeOffsetBox& offset_box, blueprint::Scale scale) noexcept -> b2ShapeId;

	// scale == sprite.scale
	// size = offset_box.size * scale
	// center = offset_box.offset * scale
	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::PhysicsShapeOffsetBox& offset_box, const blueprint::Sprite& sprite) noexcept -> b2ShapeId;

	// scale == sprite_animation.scale
	// size = offset_box.size * scale
	// center = offset_box.offset * scale
	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::PhysicsShapeOffsetBox& offset_box, const blueprint::SpriteAnimation& sprite_animation) noexcept -> b2ShapeId;
}
