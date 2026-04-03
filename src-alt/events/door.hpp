// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::events::door
{
	// 请求开启所有门(如果可以开启)
	class OpenRequest final {};

	// 请求关闭所有门(如果可以关闭)
	class CloseRequest final {};

	class Contacted final
	{
	public:
		entt::entity door;
		entt::entity entity;
	};

	class SensorContacted final
	{
	public:
		entt::entity door;
		entt::entity entity;
	};
}
