// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/door.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::factory
{
	class Door final
	{
	public:
		static auto spawn(entt::registry& registry, const blueprint::Door& door) noexcept -> entt::entity;

		static auto destroy_all(entt::registry& registry) noexcept -> void;
	};
}
