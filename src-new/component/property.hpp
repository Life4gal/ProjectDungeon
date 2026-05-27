// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

namespace pd::component::property
{
	// 当前生命值
	class Health final
	{
	public:
		float health;
	};

	// 最大生命值
	class HealthMax final
	{
	public:
		float health;
	};

	// 当前魔法值
	class Mana final
	{
	public:
		float mana;
	};

	// 最大魔法值
	class ManaMax final
	{
	public:
		float mana;
	};
}
