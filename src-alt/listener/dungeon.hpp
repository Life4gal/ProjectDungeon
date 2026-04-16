// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

namespace pd::listener
{
	class Dungeon final
	{
	public:
		static auto subscribe(entt::registry& registry) noexcept -> void;

		static auto unsubscribe(entt::registry& registry) noexcept -> void;
	};
}
