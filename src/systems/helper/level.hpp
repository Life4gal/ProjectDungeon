// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>

#include <entt/fwd.hpp>

namespace pd::config
{
	class AnimationSet;
	class TileSet;

	class Level;
}

namespace pd::systems::helper
{
	class Level final
	{
	public:
		// =====================================
		// 关卡
		// =====================================

		// 创建关卡
		static auto create(entt::registry& registry, const config::Level& level) noexcept -> entt::entity;

		// 销毁关卡
		static auto destroy(entt::registry& registry, entt::entity level_entity) noexcept -> void;

		// 加入关卡
		static auto join(entt::registry& registry, entt::entity level_entity, entt::entity player_entity) noexcept -> bool;

		// =====================================
		// 关卡(房间)事件
		// =====================================

		// 更新当前关卡(房间)
		static auto on_update(entt::registry& registry, entt::entity level_entity) noexcept -> void;

		// 触发器
		static auto on_trigger_contact(entt::registry& registry, entt::entity level_entity, entt::entity trigger_entity, entt::entity other_entity) noexcept -> void;

		// 钥匙
		static auto on_key_contact(entt::registry& registry, entt::entity level_entity, entt::entity key_entity, entt::entity other_entity) noexcept -> void;

		// 关闭的门
		static auto on_locked_door_contact(entt::registry& registry, entt::entity level_entity, entt::entity door_entity, entt::entity other_entity) noexcept -> void;

		// 开启的门
		static auto on_unlocked_door_contact(entt::registry& registry, entt::entity level_entity, entt::entity door_entity, entt::entity other_entity) noexcept -> void;

	private:
		// 创建房间
		static auto create_room(entt::registry& registry, entt::entity level_entity, const std::string& room_id) noexcept -> entt::entity;

		// 销毁房间
		// 关卡应该不会销毁房间,而是在销毁关卡时一次性销毁所有房间
		// 也许以后我们可以有关卡房间变化的需求?
		// static auto destroy_room(entt::registry& registry, entt::entity level_entity, const std::string& room_id) noexcept -> void;

		// 切换到指定房间
		static auto switch_room(
			entt::registry& registry,
			entt::entity level_entity,
			entt::entity this_room_entity,
			entt::entity target_room_entity,
			entt::entity player_entity
		) noexcept -> void;
	};
}
