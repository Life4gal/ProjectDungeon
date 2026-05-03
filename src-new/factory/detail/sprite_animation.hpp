// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/sprite_animation.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::factory::detail
{
	auto attach(entt::registry& registry, entt::entity entity, const blueprint::SpriteAnimation& sprite_animation) noexcept -> void;
}
