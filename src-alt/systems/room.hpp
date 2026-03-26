// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <components/room.hpp>

#include <events/room.hpp>

#include <SFML/System/Vector2.hpp>

namespace pd::systems
{
	class Room final
	{
	public:
		using room_position_type = sf::Vector2u;

		using cache_tiles = components::room::CacheTiles;
		using cache_doors = components::room::CacheDoors;
		using cache_chests = components::room::CacheChests;
		using cache_destroyable_objects = components::room::CacheDestroyableObjects;
		using cache_items = components::room::CacheItems;
		using cache_corpses = components::room::CacheCorpses;
		using cache_blood_stains = components::room::CacheBloodStains;

		using room_type = components::room::RoomType;
		using room_info = components::room::RoomInfo;

		// =============================================
		// 系统创建/销毁
		// =============================================

		static auto create(entt::registry& registry, room_position_type start_position, std::uint32_t count) noexcept -> void;

		static auto destroy(entt::registry& registry) noexcept -> void;

		// =============================================
		// 事件响应
		// =============================================

		// 处理接触宝箱事件
		static auto on_contact_chest(entt::registry& registry, const events::ChestContacted& event) noexcept -> void;

		// =============================================
		// 内容获取
		// =============================================

		// 获取目标房间信息
		[[nodiscard]] static auto get_info(entt::registry& registry, room_position_type position) noexcept -> room_info&;

		// 获取当前房间信息
		[[nodiscard]] static auto get_info(entt::registry& registry) noexcept -> room_info&;

		// 获取目标房间瓦片缓存
		[[nodiscard]] static auto get_cache_tiles(entt::registry& registry, room_position_type position) noexcept -> cache_tiles&;

		// 获取当前房间瓦片缓存
		[[nodiscard]] static auto get_cache_tiles(entt::registry& registry) noexcept -> cache_tiles&;

		// 获取目标房间门缓存
		[[nodiscard]] static auto get_cache_doors(entt::registry& registry, room_position_type position) noexcept -> cache_doors&;

		// 获取当前房间门缓存
		[[nodiscard]] static auto get_cache_doors(entt::registry& registry) noexcept -> cache_doors&;

		// 获取目标房间宝箱缓存
		[[nodiscard]] static auto get_cache_chests(entt::registry& registry, room_position_type position) noexcept -> cache_chests&;

		// 获取当前房间宝箱缓存
		[[nodiscard]] static auto get_cache_chests(entt::registry& registry) noexcept -> cache_chests&;

		// 获取目标房间可破坏物缓存
		[[nodiscard]] static auto get_cache_destroyable_objects(entt::registry& registry, room_position_type position) noexcept -> cache_destroyable_objects&;

		// 获取当前房间可破坏物缓存
		[[nodiscard]] static auto get_cache_destroyable_objects(entt::registry& registry) noexcept -> cache_destroyable_objects&;

		// 获取目标房间物品缓存
		[[nodiscard]] static auto get_cache_items(entt::registry& registry, room_position_type position) noexcept -> cache_items&;

		// 获取当前房间物品缓存
		[[nodiscard]] static auto get_cache_items(entt::registry& registry) noexcept -> cache_items&;

		// 获取目标房间尸体缓存
		[[nodiscard]] static auto get_cache_corpses(entt::registry& registry, room_position_type position) noexcept -> cache_corpses&;

		// 获取当前房间尸体缓存
		[[nodiscard]] static auto get_cache_corpses(entt::registry& registry) noexcept -> cache_corpses&;

		// 获取目标房间血迹缓存
		[[nodiscard]] static auto get_cache_blood_stains(entt::registry& registry, room_position_type position) noexcept -> cache_blood_stains&;

		// 获取当前房间血迹缓存
		[[nodiscard]] static auto get_cache_blood_stains(entt::registry& registry) noexcept -> cache_blood_stains&;

		// 获取当前房间坐标
		[[nodiscard]] static auto get_position(entt::registry& registry) noexcept -> sf::Vector2u;

		// =============================================
		// 下面这些接口实在没有啥更合适的地方放了,就放在Room里了
		// =============================================

		using collision_logical = components::tile::CollisionLogical;

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
