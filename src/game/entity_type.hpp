// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cstdint>

namespace pd
{
	// 实体类型
	// 这些值会用做碰撞检测过滤,小心设置!
	enum class EntityType : std::uint64_t
	{
		// NONE = 0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000,

		// [0~7]

		// 地板
		FLOOR = 0b0000'0001,

		// [8~15]

		// 墙壁
		WALL = 0b0000'0001'0000'0000,
		// 墙壁装饰物(叠加在墙壁上)
		WALL_DECORATION = 0b0000'0010'0000'0000,
		// 门
		DOOR = 0b0000'0100'0000'0000,

		// [16~23]

		// 地板装饰物(不影响移动, 感应器)
		FLOOR_DECORATION = 0b0000'0001'0000'0000'0000'0000,
		// 地板洞(阻止地面移动,但是可以飞过)
		FLOOR_HOLE = 0b0000'0010'0000'0000'0000'0000,
		// 矮地形(阻止地面移动,但是可以飞过)
		LOW_TERRAIN = 0b0000'0100'0000'0000'0000'0000,
		// 高地形(阻止地面移动,不能飞过)
		HIGH_TERRAIN = 0b0000'1000'0000'0000'0000'0000,

		// [24~31]

		// 地面敌人
		ENEMY_GROUND = 0b0000'0001'0000'0000'0000'0000'0000'0000,
		// 空中敌人
		ENEMY_AERIAL = 0b0000'0010'0000'0000'0000'0000'0000'0000,
		// 地面玩家
		PLAYER_GROUND = 0b0000'0100'0000'0000'0000'0000'0000'0000,
		// 空中玩家
		PLAYER_AERIAL = 0b0000'1000'0000'0000'0000'0000'0000'0000,

		// [32~39]

		// todo: 武器是否需要区分空中/地面?还是说空中/地面仅影响移动?

		// 近战武器
		MELEE = 0b0000'0001'0000'0000'0000'0000'0000'0000'0000'0000,
		// 远程武器(飞弹)
		PROJECTILE = 0b0000'0010'0000'0000'0000'0000'0000'0000'0000'0000,

		// [40~47]

		// 特效
		EFFECT = 0b0000'0001'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000,

		// [48~55]

		// [56~63]

		MASK = 0b1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111
	};

	class CollisionMask final
	{
	public:
		// 墙壁碰撞掩码
		constexpr static auto wall =
				// 和敌人/玩家/近战武器/远程武器/特效碰撞
				std::to_underlying(EntityType::ENEMY_GROUND) |
				std::to_underlying(EntityType::ENEMY_AERIAL) |
				std::to_underlying(EntityType::PLAYER_GROUND) |
				std::to_underlying(EntityType::PLAYER_AERIAL) |
				std::to_underlying(EntityType::MELEE) |
				std::to_underlying(EntityType::PROJECTILE) |
				std::to_underlying(EntityType::EFFECT);

		// 开启的门碰撞掩码(传感器)
		constexpr static auto door_unlocked =
				// 和玩家碰撞
				std::to_underlying(EntityType::PLAYER_GROUND) |
				std::to_underlying(EntityType::PLAYER_AERIAL);

		// 关闭的门碰撞掩码
		constexpr static auto door_locked =
				// 与墙壁相同
				wall;

		// 地板装饰物
		constexpr static auto floor_decoration =
				// 和玩家/敌人碰撞
				std::to_underlying(EntityType::ENEMY_GROUND) |
				std::to_underlying(EntityType::PLAYER_GROUND);

		// 地板洞
		constexpr static auto floor_hole =
				// 和玩家/敌人碰撞
				std::to_underlying(EntityType::ENEMY_GROUND) |
				std::to_underlying(EntityType::PLAYER_GROUND);

		// 矮地形
		constexpr static auto low_terrain =
				// 和玩家/敌人/近战武器/远程武器/特效碰撞
				std::to_underlying(EntityType::ENEMY_GROUND) |
				std::to_underlying(EntityType::PLAYER_GROUND) |
				std::to_underlying(EntityType::MELEE) |
				std::to_underlying(EntityType::PROJECTILE) |
				std::to_underlying(EntityType::EFFECT);

		// 高地形
		constexpr static auto high_terrain =
				// 和玩家/敌人/近战武器/远程武器/特效碰撞
				std::to_underlying(EntityType::ENEMY_GROUND) |
				std::to_underlying(EntityType::ENEMY_AERIAL) |
				std::to_underlying(EntityType::PLAYER_GROUND) |
				std::to_underlying(EntityType::PLAYER_AERIAL) |
				std::to_underlying(EntityType::MELEE) |
				std::to_underlying(EntityType::PROJECTILE) |
				std::to_underlying(EntityType::EFFECT);

		// 地面敌人
		constexpr static auto enemy_ground =
				// 和地板装饰物/地板洞/矮地形/高地形/地面敌人/地面玩家/近战武器/远程武器/特效碰撞
				std::to_underlying(EntityType::FLOOR_DECORATION) |
				std::to_underlying(EntityType::FLOOR_HOLE) |
				std::to_underlying(EntityType::LOW_TERRAIN) |
				std::to_underlying(EntityType::HIGH_TERRAIN) |
				std::to_underlying(EntityType::ENEMY_GROUND) |
				std::to_underlying(EntityType::PLAYER_GROUND) |
				std::to_underlying(EntityType::MELEE) |
				std::to_underlying(EntityType::PROJECTILE) |
				std::to_underlying(EntityType::EFFECT);

		// 空中敌人
		constexpr static auto enemy_aerial =
				// 和高地形/空中敌人/空中玩家/近战武器/远程武器/特效碰撞
				std::to_underlying(EntityType::HIGH_TERRAIN) |
				std::to_underlying(EntityType::ENEMY_AERIAL) |
				std::to_underlying(EntityType::PLAYER_AERIAL) |
				std::to_underlying(EntityType::MELEE) |
				std::to_underlying(EntityType::PROJECTILE) |
				std::to_underlying(EntityType::EFFECT);

		// 地面玩家
		constexpr static auto player_ground =
				// 和墙壁/门/地板装饰物/地板洞/矮地形/高地形/地面敌人/地面玩家/近战武器/远程武器/特效碰撞
				std::to_underlying(EntityType::WALL) |
				std::to_underlying(EntityType::DOOR) |
				std::to_underlying(EntityType::FLOOR_DECORATION) |
				std::to_underlying(EntityType::FLOOR_HOLE) |
				std::to_underlying(EntityType::LOW_TERRAIN) |
				std::to_underlying(EntityType::HIGH_TERRAIN) |
				std::to_underlying(EntityType::ENEMY_GROUND) |
				std::to_underlying(EntityType::PLAYER_GROUND) |
				std::to_underlying(EntityType::MELEE) |
				std::to_underlying(EntityType::PROJECTILE) |
				std::to_underlying(EntityType::EFFECT);

		// 空中玩家
		constexpr static auto player_aerial =
				// 和墙壁/门/高地形/空中敌人/空中玩家/近战武器/远程武器/特效碰撞
				std::to_underlying(EntityType::WALL) |
				std::to_underlying(EntityType::DOOR) |
				std::to_underlying(EntityType::HIGH_TERRAIN) |
				std::to_underlying(EntityType::ENEMY_AERIAL) |
				std::to_underlying(EntityType::PLAYER_AERIAL) |
				std::to_underlying(EntityType::MELEE) |
				std::to_underlying(EntityType::PROJECTILE) |
				std::to_underlying(EntityType::EFFECT);

		// 近战武器
		constexpr static auto melee =
				// 和墙壁/矮地形/高地形/敌人/玩家碰撞
				std::to_underlying(EntityType::WALL) |
				std::to_underlying(EntityType::LOW_TERRAIN) |
				std::to_underlying(EntityType::HIGH_TERRAIN) |
				std::to_underlying(EntityType::ENEMY_GROUND) |
				std::to_underlying(EntityType::ENEMY_AERIAL) |
				std::to_underlying(EntityType::PLAYER_GROUND) |
				std::to_underlying(EntityType::PLAYER_AERIAL);

		// 远程武器(飞弹)
		constexpr static auto projectile =
				// 和墙壁/矮地形/高地形/敌人/玩家碰撞
				std::to_underlying(EntityType::WALL) |
				std::to_underlying(EntityType::LOW_TERRAIN) |
				std::to_underlying(EntityType::HIGH_TERRAIN) |
				std::to_underlying(EntityType::ENEMY_GROUND) |
				std::to_underlying(EntityType::ENEMY_AERIAL) |
				std::to_underlying(EntityType::PLAYER_GROUND) |
				std::to_underlying(EntityType::PLAYER_AERIAL);
	};
}
