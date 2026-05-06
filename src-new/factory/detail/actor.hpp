// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/actor.hpp>
#include <blueprint/detail/sprite.hpp>
#include <blueprint/detail/sprite_animation.hpp>

#include <entt/entity/registry.hpp>

namespace pd::factory::detail
{
	// size == 目标实体的大小
	// origin == 目标实体的原点(一般为size/2)
	auto attach(entt::registry& registry, entt::entity entity, const blueprint::Actor& actor, blueprint::Size size, blueprint::Position origin) noexcept -> void;

	// size == sprite.size
	// origin == sprite.origin
	auto attach(entt::registry& registry, entt::entity entity, const blueprint::Actor& actor, const blueprint::Sprite& sprite) noexcept -> void;

	// size == sprite_animation.size
	// origin == sprite_animation.origin
	auto attach(entt::registry& registry, entt::entity entity, const blueprint::Actor& actor, const blueprint::SpriteAnimation& sprite_animation) noexcept -> void;
}
