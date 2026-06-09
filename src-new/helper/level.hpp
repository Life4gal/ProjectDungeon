// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/layout.hpp>

#include <entt/fwd.hpp>

namespace pd::helper
{
	class Level final
	{
	public:
		// 注册房间
		// 创建房间实体后需要注册房间,否则无法进入邻居房间
		static auto register_room(entt::registry& registry, blueprint::LayoutPosition position, entt::entity room) noexcept -> void;
	};
}
