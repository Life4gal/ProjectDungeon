// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

namespace pd::event::player
{
	// 仅在非玩家控制的移动时才触发该事件
	class MoveTo final
	{
	public:
		float x;
		float y;
	};

	// 仅在非玩家控制的移动时才触发该事件
	class Translate final
	{
	public:
		float x;
		float y;
	};
}
