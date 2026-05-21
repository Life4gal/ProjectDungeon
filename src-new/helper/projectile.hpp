// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

namespace pd::helper
{
	class Projectile final
	{
	public:
		// 接触其他实体
		//
		// Event:
		//  event::actor::Damaged (Actor::hurt)
		static auto contact(entt::registry& registry, entt::entity projectile, entt::entity other) noexcept -> void;
	};
}
