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

		// =====================================
		// 房间
		// =====================================

		// 创建房间
		static auto create_room(entt::registry& registry, entt::entity level_entity, const std::string& room_id) noexcept -> bool;

		// 销毁房间
		static auto destroy_room(entt::registry& registry, entt::entity level_entity, const std::string& room_id) noexcept -> void;

		// 切换到指定房间(如果房间不存在则切换失败,不会创建)
		static auto switch_room(entt::registry& registry, entt::entity level_entity, const std::string& room_id) noexcept -> bool;

		// 检查当前房间是否已清空(无敌人)
		[[nodiscard]] static auto is_room_cleared(entt::registry& registry) noexcept -> bool;

		// 更新当前房间(检查是否已经清除所有敌人,如果是则开启所有门)
		static auto update_room(entt::registry& registry) noexcept -> void;
	};
}
