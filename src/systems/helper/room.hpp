// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>
#include <span>

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
	class KeyTile;
	class DoorTile;
	class TileSet;

	class Wall;
	class Floor;
	class Decoration;
	class Trigger;
	class Key;
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
			const config::AnimationSet& animation_set,
			const config::TileSet& tile_set
		) noexcept -> entt::entity;

		static auto destroy(entt::registry& registry, entt::entity room_entity) noexcept -> void;

		// =====================================
		// 房间事件
		// =====================================

		// 进入房间
		// todo: 我们不希望房间依赖关卡(房间访问属于关卡的组件),所以将玩家当前持有的钥匙传入
		static auto on_enter(entt::registry& registry, entt::entity room_entity, entt::entity player_entity, std::span<const std::string> keys) noexcept -> void;

		// 离开房间
		static auto on_exit(entt::registry& registry, entt::entity room_entity, entt::entity player_entity) noexcept -> void;

		// 房间更新
		static auto on_update(entt::registry& registry, entt::entity room_entity) noexcept -> void;

		// 钥匙被收集(玩家接触了钥匙)
		// 这会销毁钥匙实体
		static auto on_key_collected(entt::registry& registry, entt::entity room_entity, entt::entity key_entity) noexcept -> void;

	private:
		// =============================================
		// 房间
		// =============================================

		// 隐藏所有实体
		static auto hide(entt::registry& registry, entt::entity room_entity) noexcept -> void;

		// 显示所有实体
		static auto show(entt::registry& registry, entt::entity room_entity) noexcept -> void;

		// 应用门的钥匙
		// 应用钥匙并不意味着门开启,仅是移除了门所需钥匙
		static auto apply_key(entt::registry& registry, entt::entity room_entity, const std::string& key_name) noexcept -> void;

		// 应用门的钥匙
		// 应用钥匙并不意味着门开启,仅是移除了门所需钥匙
		static auto apply_key(entt::registry& registry, entt::entity room_entity, entt::entity key_entity) noexcept -> void;

		// 开启所有不需要钥匙的门
		// 在进入该房间/收集钥匙时,会移除对应门的钥匙(但是其依然保持锁住状态, 直到解锁)
		static auto unlock_doors(entt::registry& registry, entt::entity room_entity) noexcept -> void;

		// 锁住所有门
		// 一般用于进入一个房间
		static auto lock_doors(entt::registry& registry, entt::entity room_entity) noexcept -> void;

		// 解锁门(移除Locked组件,将物理体改为传感器)
		static auto unlock_door(entt::registry& registry, entt::entity door_entity) noexcept -> void;

		// 锁定门(添加Locked组件,将物理体改为实体)
		static auto lock_door(entt::registry& registry, entt::entity door_entity) noexcept -> void;

		// =============================================
		// 墙壁
		// =============================================

		static auto create_wall(
			entt::registry& registry,
			const config::Wall& wall,
			const config::WallTile& wall_tile,
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
			const config::AnimationSet& animation_set,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1},
			sf::Angle rotation = sf::degrees(0)
		) noexcept -> entt::entity;

		static auto destroy_trigger(entt::registry& registry, entt::entity trigger_entity) noexcept -> void;

		// =============================================
		// 钥匙
		// =============================================

		// 创建钥匙
		[[nodiscard]] static auto create_key(
			entt::registry& registry,
			const config::Key& key,
			const config::KeyTile& key_tile,
			const config::AnimationSet& animation_set,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1},
			sf::Angle rotation = sf::degrees(0)
		) noexcept -> entt::entity;

		// 销毁钥匙(也可以说是收集钥匙)
		static auto destroy_key(entt::registry& registry, entt::entity key_entity) noexcept -> void;

		// =============================================
		// 门
		// =============================================

		static auto create_locked_door(
			entt::registry& registry,
			const config::Door& door,
			const config::DoorTile& door_tile,
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
	};
}
