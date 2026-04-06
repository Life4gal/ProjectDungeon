// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::events::renderable
{
	class Hide final
	{
	public:
		entt::entity entity;
	};

	class Show final
	{
	public:
		entt::entity entity;
	};
}
