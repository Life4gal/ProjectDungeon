// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/level.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::factory
{
	class Level final
	{
	public:
		static auto create(entt::registry& registry, const blueprint::Level& level) noexcept -> void;

		static auto destroy(entt::registry& registry) noexcept -> void;
	};
}
