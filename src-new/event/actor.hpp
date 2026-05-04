// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::event::actor
{
	// 受到伤害
	class Hurt final
	{
	public:
		entt::entity attacker;
		entt::entity victim;

		float damage;
	};

	// 死亡
	class Dead final
	{
	public:
		entt::entity attacker;
		entt::entity victim;
	};
}
