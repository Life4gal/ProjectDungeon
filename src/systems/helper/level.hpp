// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>

#include <entt/fwd.hpp>

namespace pd::config
{
	class Level;
	class TileSet;
}

namespace pd::systems::helper
{
	class Level final
	{
	public:
		// 加载关卡
		static auto load_level(entt::registry& registry, const config::Level& level, const config::TileSet& tileset) noexcept -> bool;

		// 卸载当前关卡
		static auto unload_level(entt::registry& registry) noexcept -> void;

		// 切换到指定房间
		static auto switch_room(entt::registry& registry, const std::string& room_id) noexcept -> bool;

		// 检查当前房间是否已清空(无敌人)
		[[nodiscard]] static auto is_room_cleared(entt::registry& registry) noexcept -> bool;

		// 检查当前房间状态
		static auto check_room(entt::registry& registry) noexcept -> void;

		// 获取当前房间id
		[[nodiscard]] static auto get_room_id(entt::registry& registry) noexcept -> std::string_view;
	};
}
