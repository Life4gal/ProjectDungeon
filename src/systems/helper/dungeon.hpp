// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>

#include <entt/fwd.hpp>

namespace pd::config
{
	class Dungeon;
}

namespace pd::systems::helper
{
	class Dungeon final
	{
	public:
		// 加载地下城
		static auto load(entt::registry& registry, const std::filesystem::path& path) noexcept -> bool;

		// 加载地下城
		static auto load(entt::registry& registry, config::Dungeon dungeon) noexcept -> bool;

		// 卸载地下城
		static auto unload(entt::registry& registry) noexcept -> void;

		// =====================================
		// 关卡(房间)事件
		// =====================================

		// 更新当前关卡(房间)
		static auto on_update(entt::registry& registry) noexcept -> void;

		// 触发器
		static auto on_trigger_contact(entt::registry& registry, entt::entity trigger_entity, entt::entity other_entity) noexcept -> void;

		// 钥匙
		static auto on_key_contact(entt::registry& registry, entt::entity key_entity, entt::entity other_entity) noexcept -> void;

		// 关闭的门
		static auto on_locked_door_contact(entt::registry& registry, entt::entity door_entity, entt::entity other_entity) noexcept -> void;

		// 开启的门
		static auto on_unlocked_door_contact(entt::registry& registry, entt::entity door_entity, entt::entity other_entity) noexcept -> void;

	private:
		// =====================================
		// 关卡
		// =====================================

		[[nodiscard]] static auto level(entt::registry& registry) noexcept -> entt::entity;
	};
}
