// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

namespace pd::blueprint
{
	class Actor final
	{
	public:
		// 最大生命值
		float health;
		// 最大魔法值
		float mana;
		// 最大移动速度
		// 像素/秒
		float speed;
	};
}
