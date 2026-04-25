// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

#include <blueprint/floor.hpp>

namespace pd::factory
{
	class Floor final
	{
	public:
		static auto spawn(entt::registry& registry, const blueprint::Floor& floor) noexcept -> entt::entity;

		static auto destroy(entt::registry& registry, entt::entity entity) noexcept -> void;

		static auto destroy_all(entt::registry& registry) noexcept -> void;
	};
}
