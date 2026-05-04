// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::event::door
{
	// 请求开启指定门
	class RequestOpen final
	{
	public:
		entt::entity door;
	};

	// 请求关闭指定门
	class RequestClose final
	{
	public:
		entt::entity door;
	};
}
