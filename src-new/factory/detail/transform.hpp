// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/position.hpp>
#include <blueprint/detail/sprite.hpp>
#include <blueprint/detail/sprite_animation.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::factory::detail
{
	// position == 目标实体的位置
	// scale == 目标实体的缩放
	// 0(degree) == 目标实体的旋转
	auto attach(entt::registry& registry, entt::entity entity, blueprint::Position position, blueprint::Scale scale) noexcept -> void;

	// scale == sprite.scale
	auto attach(entt::registry& registry, entt::entity entity, blueprint::Position position, const blueprint::Sprite& sprite) noexcept -> void;

	// scale == sprite_animation.scale
	auto attach(entt::registry& registry, entt::entity entity, blueprint::Position position, const blueprint::SpriteAnimation& sprite_animation) noexcept -> void;
}
