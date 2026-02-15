// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <box2d/id.h>

namespace pd::ctx
{
	class PhysicsWorld final
	{
	public:
		[[nodiscard]] static auto id(entt::registry& registry) noexcept -> b2WorldId;
	};
}
