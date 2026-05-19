// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/sprite.hpp>

namespace pd::blueprint
{
	class Floor final
	{
	public:
		// 精灵
		Sprite sprite;

		// 生成位置
		Position position;
	};
}
