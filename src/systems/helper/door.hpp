// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>

#include <config/types.hpp>

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>

namespace pd::config
{
	class Tile;
}

namespace pd::systems::helper
{
	class Door final
	{
	public:
		// 创建一个关闭的门
		static auto spawn_locked(
			entt::registry& registry,
			const config::Tile& tile,
			config::DoorDirection direction,
			const std::string& target_room_id,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1}
		) noexcept -> entt::entity;

		// 创建一个开启的门
		static auto spawn_unlocked(
			entt::registry& registry,
			const config::Tile& tile,
			config::DoorDirection direction,
			const std::string& target_room_id,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1}
		) noexcept -> entt::entity;

		// 指定门钥匙
		static auto set_key(entt::registry& registry, entt::entity entity, entt::entity key_entity) noexcept -> void;

		// 解锁门(移除Locked组件,将物理体改为传感器)
		static auto unlock(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 锁定门(添加Locked组件,将物理体改为阻挡)
		static auto lock(entt::registry& registry, entt::entity entity) noexcept -> void;
	};
}
