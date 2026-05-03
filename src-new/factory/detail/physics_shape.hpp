// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/physics_shape.hpp>
#include <blueprint/detail/transform.hpp>

#include <box2d/id.h>

namespace pd::factory::detail
{
	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::Transform& transform, const blueprint::PhysicsShapeCircle& circle) noexcept -> b2ShapeId;

	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::Transform& transform, const blueprint::PhysicsShapeCapsule& capsule) noexcept -> b2ShapeId;

	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::Transform& transform, const blueprint::PhysicsShapeBox& box) noexcept -> b2ShapeId;

	[[nodiscard]] auto create(b2BodyId body_id, const blueprint::Transform& transform, const blueprint::PhysicsShapeOffsetBox& offset_box) noexcept -> b2ShapeId;
}
