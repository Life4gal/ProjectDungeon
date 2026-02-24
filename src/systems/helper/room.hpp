// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string_view>

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>

namespace pd::config
{
	class Room;
	class TileSet;
}

namespace pd::systems::helper
{
	class Room final
	{
	public:
		static auto create(
			entt::registry& registry,
			const config::Room& room,
			const config::TileSet& tileset,
			// 房间在世界中的偏移位置(像素)
			sf::Vector2f offset = {0, 0}
		) noexcept -> entt::entity;

		static auto destroy(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 获取房间id
		[[nodiscard]] static auto id(entt::registry& registry, entt::entity entity) noexcept -> std::string_view;

		// 更新敌人实体(清除已经失效的敌人实体)
		// 返回是否所有敌人实体都已经失效了
		static auto update_enemy_entities(entt::registry& registry, entt::entity entity) noexcept -> bool;

		// 开启所有门
		// todo: 需要钥匙的如何处理?
		static auto unlock_doors(entt::registry& registry, entt::entity entity) noexcept -> void;
	};
}
