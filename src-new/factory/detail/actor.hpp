// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/actor.hpp>
#include <blueprint/detail/sprite.hpp>

#include <entt/entity/registry.hpp>

namespace pd::factory::detail
{
	// FIXME: 如果animation的各个Sprite大小不一致,生命值条&魔法值条的偏移应该对应地调整,如何调整?
	auto attach(entt::registry& registry, entt::entity entity, const blueprint::Actor& actor, const blueprint::Sprite& sprite) noexcept -> void;
}
