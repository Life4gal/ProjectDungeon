// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/physics_body.hpp>
#include <blueprint/detail/position.hpp>

#include <entt/entity/fwd.hpp>

#include <box2d/id.h>

namespace pd::factory::detail
{
	// position == 物理刚体位置
	[[nodiscard]] auto create_attach(entt::registry& registry, entt::entity entity, const blueprint::PhysicsBody& physics_body, blueprint::Position position) noexcept -> b2BodyId;
}
