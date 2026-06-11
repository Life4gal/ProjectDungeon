// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/character.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::assembly
{
	class Character final
	{
	public:
		static auto make(entt::registry& registry, entt::entity entity, const blueprint::Character& character) noexcept -> void;
	};
}
