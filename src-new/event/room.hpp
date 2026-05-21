// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::event::room
{
	// 房间敌人被清空
	class Cleared final
	{
	public:
		entt::entity room;
	};

	// 离开房间
	class Leave final
	{
	public:
		entt::entity room;
	};

	// 进入房间
	class Enter final
	{
	public:
		entt::entity room;
	};
}
