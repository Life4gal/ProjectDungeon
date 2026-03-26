// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <game/constants.hpp>

#include <components/tile.hpp>
#include <components/door.hpp>
#include <components/chest.hpp>

#include <utility/fixed_matrix.hpp>

namespace pd::components::room
{
	// ==========================================
	// 非实体组件
	// ==========================================

	// 瓦片缓存
	class CacheTile final
	{
	public:
		tile::CollisionLogical collision_logical;
		tile::Transform transform;
		tile::Visual visual;
		tile::Object object;

		// 我们保存完整的Tile表,所以这里不再保存坐标
	};

	class CacheTiles final
	{
	public:
		utility::FixedMatrix<CacheTile, game::RoomHorizontalGrid, game::RoomVerticalGrid> tiles;
	};

	// 门缓存
	class CacheDoor final
	{
	public:
		door::Direction direction;
		// 其要么开启,要么关闭,不存在开启中或是关闭中状态
		bool opened;
		door::Type type;
		// 不需要缓存动画计时器,其必定已经播放完动画

		// 理论上我们可以根据direction直接算出位置
		// NORTH
		// RoomHorizontalGrid / 2, 0
		// SOUTH
		// RoomHorizontalGrid / 2, RoomVerticalGrid - 1
		// WEST
		// 0, RoomVerticalGrid / 2
		// EAST
		// RoomHorizontalGrid - 1, RoomVerticalGrid / 2
		// 不过保存这些位置也无伤大雅?
		float x;
		float y;
	};

	class CacheDoors final
	{
	public:
		std::array<CacheDoor, std::to_underlying(door::Direction::COUNT)> doors;
	};

	// 宝箱缓存
	class CacheChest final
	{
	public:
		chest::Type type;
		// 其要么开启,要么关闭,不存在开启中或是关闭中状态
		bool opened;
		// 不需要缓存动画计时器,其要么未触发,要么已触发

		float x;
		float y;
	};

	class CacheChests final
	{
	public:
		// 一个房间会有多个宝箱吗?
		std::vector<CacheChest> chests;
	};

	// 可破坏物缓存
	class CacheDestroyableObject final
	{
	public:
	};

	class CacheDestroyableObjects final
	{
	public:
		std::vector<CacheDestroyableObject> destroyable_objects;
	};

	// 物品缓存
	class CacheItem final
	{
	public:
	};

	class CacheItems final
	{
	public:
		std::vector<CacheItem> items;
	};

	// 尸体缓存
	class CacheCorpse final
	{
	public:
	};

	class CacheCorpses final
	{
	public:
		std::vector<CacheCorpse> corpses;
	};

	// 血迹缓存
	class CacheBloodStain final
	{
	public:
	};

	class CacheBloodStains final
	{
	public:
		std::vector<CacheBloodStain> blood_stains;
	};

	// 房间类型
	enum class RoomType : std::uint8_t
	{
		// 该位置未生成房间
		NONE,

		// 起始房间
		START,
		// 标准房间
		STANDARD,
		// BOSS房间
		BOSS,
		// BOSS房间门钥匙房间
		KEY,
		// 商人房间
		MERCHANT,
		// 奖励房间
		BONUS,
		// 出口房间
		EXIT,
	};

	class RoomInfo final
	{
	public:
		// 房间类型
		RoomType type;
		// 此房间是否已访问过
		bool visited;
		// 此房间是否已清理过
		bool cleared;
		// 此房间BOSS门是否打开(如果有BOSS门)
		bool boss_door_opened;
	};

	// ==========================================
	// 上下文组件
	// ==========================================

	// 所有房间信息
	class RoomInfos final
	{
	public:
		using matrix_type = utility::FixedMatrix<RoomInfo, game::DungeonFloorWidth, game::DungeonFloorHeight>;

		matrix_type infos;
	};

	// 所有房间瓦片缓存
	class RoomCacheTiles final
	{
	public:
		using matrix_type = utility::FixedMatrix<CacheTiles, game::DungeonFloorWidth, game::DungeonFloorHeight>;

		matrix_type caches;
	};

	// 所有房间门缓存
	class RoomCacheDoors final
	{
	public:
		using matrix_type = utility::FixedMatrix<CacheDoors, game::DungeonFloorWidth, game::DungeonFloorHeight>;

		matrix_type caches;
	};

	// 所有房间宝箱缓存
	class RoomCacheChests final
	{
	public:
		using matrix_type = utility::FixedMatrix<CacheChests, game::DungeonFloorWidth, game::DungeonFloorHeight>;

		matrix_type caches;
	};

	// 所有房间可破坏物缓存
	class RoomCacheDestroyableObjects final
	{
	public:
		using matrix_type = utility::FixedMatrix<CacheDestroyableObjects, game::DungeonFloorWidth, game::DungeonFloorHeight>;

		matrix_type caches;
	};

	// 所有房间物品缓存
	class RoomCacheItems final
	{
	public:
		using matrix_type = utility::FixedMatrix<CacheItems, game::DungeonFloorWidth, game::DungeonFloorHeight>;

		matrix_type caches;
	};

	// 所有房间尸体缓存
	class RoomCacheCorpses final
	{
	public:
		using matrix_type = utility::FixedMatrix<CacheCorpses, game::DungeonFloorWidth, game::DungeonFloorHeight>;

		matrix_type caches;
	};

	// 所有房间血迹缓存
	class RoomCacheBloodStains final
	{
	public:
		using matrix_type = utility::FixedMatrix<CacheBloodStains, game::DungeonFloorWidth, game::DungeonFloorHeight>;

		matrix_type caches;
	};

	// 当前所在房间坐标
	class RoomCurrentPosition final
	{
	public:
		std::uint32_t x;
		std::uint32_t y;
	};

	// ==========================================
	// 实体组件
	// ==========================================

	// 房间似乎完全不需要实体?
}
