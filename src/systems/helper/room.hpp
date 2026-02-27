// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>

namespace pd::config
{
	class AnimationFrame;
	class Animation;
	class AnimationSet;

	class WallTile;
	class FloorTile;
	class DecorationTile;
	class TriggerTile;
	class DoorTile;
	class TileSet;

	class Wall;
	class Floor;
	class Decoration;
	class Trigger;
	class Door;
	class Room;
}

namespace pd::systems::helper
{
	class Room final
	{
	public:
		// =============================================
		// 房间
		// =============================================

		[[nodiscard]] static auto create(
			entt::registry& registry,
			const config::Room& room,
			// config::Dungeon::animation_set
			const config::AnimationSet& animation_set,
			// config::Dungeon::tile_set
			const config::TileSet& tile_set
		) noexcept -> entt::entity;

		static auto destroy(entt::registry& registry, entt::entity room_entity) noexcept -> void;

		// 检查房间是否已清空(无敌人)
		[[nodiscard]] static auto is_cleared(entt::registry& registry, entt::entity room_entity) noexcept -> bool;

		// 更新敌人实体(清除已经失效的敌人实体)
		// 返回是否所有敌人实体都已经失效了
		static auto update_enemy_entities(entt::registry& registry, entt::entity room_entity) noexcept -> bool;

		// 开启所有门
		// todo: 需要钥匙的如何处理?
		static auto unlock_doors(entt::registry& registry, entt::entity room_entity) noexcept -> void;

		// =============================================
		// 墙壁
		// =============================================

		static auto create_wall(
			entt::registry& registry,
			const config::Wall& wall,
			const config::WallTile& wall_tile,
			// config::Dungeon::animation_set
			const config::AnimationSet& animation_set,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1},
			sf::Angle rotation = sf::degrees(0)
		) noexcept -> entt::entity;

		static auto destroy_wall(entt::registry& registry, entt::entity wall_entity) noexcept -> void;

		// =============================================
		// 地板
		// =============================================

		static auto create_floor(
			entt::registry& registry,
			const config::Floor& floor,
			const config::FloorTile& floor_tile,
			// config::Dungeon::animation_set
			const config::AnimationSet& animation_set,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1},
			sf::Angle rotation = sf::degrees(0)
		) noexcept -> entt::entity;

		static auto destroy_floor(entt::registry& registry, entt::entity floor_entity) noexcept -> void;

		// =============================================
		// 装饰物
		// =============================================

		static auto create_decoration(
			entt::registry& registry,
			const config::Decoration& decoration,
			const config::DecorationTile& decoration_tile,
			// config::Dungeon::animation_set
			const config::AnimationSet& animation_set,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1},
			sf::Angle rotation = sf::degrees(0)
		) noexcept -> entt::entity;

		static auto destroy_decoration(entt::registry& registry, entt::entity decoration_entity) noexcept -> void;

		// =============================================
		// 触发器
		// =============================================

		static auto create_trigger(
			entt::registry& registry,
			const config::Trigger& trigger,
			const config::TriggerTile& trigger_tile,
			// config::Dungeon::animation_set
			const config::AnimationSet& animation_set,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1},
			sf::Angle rotation = sf::degrees(0)
		) noexcept -> entt::entity;

		static auto destroy_trigger(entt::registry& registry, entt::entity trigger_entity) noexcept -> void;

		// ================
		// 触发器接触事件
		// ================

		static auto on_trigger_contact(entt::registry& registry, entt::entity trigger_entity, entt::entity other_entity) noexcept -> void;

		// =============================================
		// 门
		// =============================================

		static auto create_locked_door(
			entt::registry& registry,
			const config::Door& door,
			const config::DoorTile& door_tile,
			// config::Dungeon::animation_set
			const config::AnimationSet& animation_set,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1}
		) noexcept -> entt::entity;

		static auto create_unlocked_door(
			entt::registry& registry,
			const config::Door& door,
			const config::DoorTile& door_tile,
			// config::Dungeon::animation_set
			const config::AnimationSet& animation_set,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1}
		) noexcept -> entt::entity;

		static auto destroy_door(entt::registry& registry, entt::entity door_entity) noexcept -> void;

		// 指定门钥匙
		static auto set_door_key(entt::registry& registry, entt::entity door_entity, entt::entity key_entity) noexcept -> void;

		// 解锁门(移除Locked组件,将物理体改为传感器)
		static auto unlock_door(entt::registry& registry, entt::entity door_entity) noexcept -> void;

		// 锁定门(添加Locked组件,将物理体改为实体)
		static auto lock_door(entt::registry& registry, entt::entity door_entity) noexcept -> void;

		// ================
		// 门接触事件
		// ================

		static auto on_locked_door_contact(entt::registry& registry, entt::entity door_entity, entt::entity other_entity) noexcept -> void;

		static auto on_unlocked_door_contact(entt::registry& registry, entt::entity door_entity, entt::entity other_entity) noexcept -> void;
	};
}
