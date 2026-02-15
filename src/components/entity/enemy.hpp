// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <config/types.hpp>

namespace pd::components::enemy
{
	// 敌人类型
	class Type final
	{
	public:
		config::EnemyType type;
	};

	// 敌人触碰玩家造成的伤害
	class CollideDamage final
	{
	public:
		float damage;
	};
}
