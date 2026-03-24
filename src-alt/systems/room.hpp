// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <components/room.hpp>

#include <entt/fwd.hpp>

namespace pd::systems
{
	class Room final
	{
	public:
		using size_type = components::room::Room::size_type;

		static auto create(entt::registry& registry) noexcept -> void;

		static auto destroy(entt::registry& registry) noexcept -> void;

		// 开启所有门(除非其锁住了)
		static auto open_doors(entt::registry& registry) noexcept -> void;

		// 关闭所有门
		static auto close_doors(entt::registry& registry) noexcept -> void;

		[[nodiscard]] static auto tile(entt::registry& registry, size_type x, size_type y) noexcept -> const components::room::Tile*;

		// 是否可以行走
		// 一般单指地板类型
		[[nodiscard]] static auto is_walkable(entt::registry& registry, size_type x, size_type y) noexcept -> bool;

		// 是否可以飞越
		// 一般单指非墙壁类型
		[[nodiscard]] static auto is_flyable(entt::registry& registry, size_type x, size_type y) noexcept -> bool;

		// 是否可以穿越
		// 一般指非墙壁类型和非障碍物类型
		// 其比飞行更低,例如飞弹
		[[nodiscard]] static auto is_passable(entt::registry& registry, size_type x, size_type y) noexcept -> bool;
	};
}
