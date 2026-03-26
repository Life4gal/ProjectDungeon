// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

namespace pd::systems
{
	class Door
	{
	public:
		// 开启所有门(除非其锁住了)
		static auto open(entt::registry& registry) noexcept -> void;

		// 关闭所有门
		static auto close(entt::registry& registry) noexcept -> void;
	};
}
