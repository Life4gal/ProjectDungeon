// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/transform.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::assembly
{
	class Transform final
	{
	public:
		static auto make(
			entt::registry& registry,
			entt::entity entity,
			blueprint::Position position,
			blueprint::Scale scale = {.x = 1, .y = 1},
			blueprint::Rotation rotation = {.rotation = 0}
		) noexcept -> void;
	};
}
