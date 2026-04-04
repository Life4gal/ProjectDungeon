// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <events/room.hpp>

#include <components/tile.hpp>

#include <entt/fwd.hpp>

namespace pd::systems
{
	class Tile final
	{
	public:
		using collision_logical = components::tile::CollisionLogical;

		// =============================================
		// 事件响应
		// =============================================
		// 订阅事件
		static auto subscribe_events(entt::registry& registry) noexcept -> void;

		// 取消订阅事件
		static auto unsubscribe_events(entt::registry& registry) noexcept -> void;

	private:
		static auto on_disable(entt::registry& registry, const events::room::DisableTile& event) noexcept -> void;

		static auto on_enable(entt::registry& registry, const events::room::EnableTile& event) noexcept -> void;

	public:
		// =============================================
		// 创建/销毁实体
		// =============================================

		// 创建瓦片
		// todo: 参数?
		static auto create(entt::registry& registry) noexcept -> entt::entity;

		// 销毁瓦片
		static auto destroy(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 销毁所有瓦片
		static auto destroy_all(entt::registry& registry) noexcept -> void;

		// =============================================
		// 辅助接口(也许不需要公开?)
		// =============================================

		// 是否可以行走
		// 一般单指地板类型
		[[nodiscard]] static auto is_walkable(collision_logical collision_logical) noexcept -> bool;

		// 是否可以飞越
		// 一般单指非墙壁类型
		[[nodiscard]] static auto is_flyable(collision_logical collision_logical) noexcept -> bool;

		// 是否可以穿越
		// 一般指非墙壁类型和非障碍物类型
		// 其比飞行更低,例如飞弹
		[[nodiscard]] static auto is_passable(collision_logical collision_logical) noexcept -> bool;
	};
}
