// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::event::door
{
	// 门被接触
	class Contacted final
	{
	public:
		entt::entity door;
		entt::entity other;
	};

	// 门传感器激活
	class Sensed final
	{
	public:
		entt::entity door;
		// 仅运行玩家激活门传感器
		entt::entity player;
	};

	// 门被开启
	class Opened final
	{
	public:
		entt::entity door;
	};

	// 门被关闭
	class Closed final
	{
	public:
		entt::entity door;
	};
}
