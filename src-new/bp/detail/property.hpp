// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

namespace pd::bp
{
	class Property final
	{
	public:
		// 最大生命值
		float health;

		// 最大魔法值
		float mana;

		// 是否无敌(生命值永远不会减少)
		bool invincible;
		// 是否无限魔法值(魔法值永远不会减少)
		bool infinity_mana;
		//
	};
}
