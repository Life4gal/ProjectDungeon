// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/animation.hpp>

#include <entt/entity/registry.hpp>

namespace pd::assembly
{
	class Animation final
	{
	public:
		static auto make(entt::registry& registry, entt::entity entity, const blueprint::Animation& animation) noexcept -> void;
	};
}
