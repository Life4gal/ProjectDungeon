// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::event::actor
{
	// 被伤害
	class Damaged final
	{
	public:
		entt::entity victim;
		entt::entity attacker;
		float damage;
	};

	// 被杀死
	class Slayed final
	{
	public:
		entt::entity victim;
		entt::entity attacker;
	};
}
