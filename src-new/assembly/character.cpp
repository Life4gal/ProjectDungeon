// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <assembly/character.hpp>

#include <assembly/transform.hpp>
#include <assembly/detail/animation.hpp>
#include <assembly/collision.hpp>
#include <assembly/detail/property.hpp>
#include <assembly/detail/name.hpp>

namespace pd::assembly
{
	auto Character::make(entt::registry& registry, const entt::entity entity, const blueprint::Character& character) noexcept -> void
	{
		Transform::make(registry, entity, character.position);
		Animation::make(registry, entity, character.animation);
		Collision::make(registry, entity, character.collision, character.position);
		Property::make(registry, entity, character.property);
		Name::make(registry, entity, character.name);
	}
}
