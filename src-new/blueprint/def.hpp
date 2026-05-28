// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <utility>

namespace pd::blueprint
{
	enum class Direction : std::uint8_t
	{
		NORTH = 0b00,
		SOUTH = 0b01,
		WEST = 0b10,
		EAST = 0b11,
	};

	enum class DirectionMask : std::uint8_t
	{
		NONE = 0,

		NORTH = 0b0001,
		SOUTH = 0b0010,
		WEST = 0b0100,
		EAST = 0b1000
	};

	// [[nodiscard]] constexpr auto operator-(const Direction direction) noexcept -> Direction
	// {
	// 	return static_cast<Direction>(std::to_underlying(direction) ^ 0b01);
	// }

	// 渲染层(渲染顺序)
	enum class RenderLayer : std::uint32_t
	{
		// [0~7]

		FLOOR = 1uz << 0,
		WALL = 1uz << 1,
		DOOR = 1uz << 2,

		// [8~15]

		ENEMY = 1uz << 8,
		PLAYER = 1uz << 9,

		// [16~23]

		CLAW = 1uz << 16,
		PROJECTILE = 1 << 17,

		// [24~31]
		//
	};

	// 碰撞体类型
	// b2ShapeDef::filter -> categoryBits
	enum class CollisionCategory : std::uint64_t // NOLINT(performance-enum-size)
	{
		// [0~7]

		// 墙壁
		WALL = 1u << 0,

		// 门
		DOOR = 1u << 1,
		// 门传感器
		DOOR_SENSOR = 1u << 2,

		// [8~15]

		// 玩家
		PLAYER = 1u << 8,

		// 敌人
		ENEMY = 1u << 9,

		// [16~23]

		// 爪(近战武器)
		CLAW = 1u << 16,

		// 飞弹(远程武器)
		PROJECTILE = 1u << 17,

		// [24~31]
		//
	};

	// 碰撞体掩码
	// b2ShapeDef::filter -> maskBits
	enum class CollisionMask : std::underlying_type_t<CollisionCategory> // NOLINT(performance-enum-size)
	{
		// 不与任何物体发生碰撞
		NONE = 0,

		// 墙壁
		WALL = //
		std::to_underlying(CollisionCategory::PLAYER) | //
		std::to_underlying(CollisionCategory::ENEMY) | //
		std::to_underlying(CollisionCategory::PROJECTILE) //
		,

		// 关闭的门
		DOOR = //
		std::to_underlying(CollisionCategory::PLAYER) | //
		std::to_underlying(CollisionCategory::ENEMY) | //
		std::to_underlying(CollisionCategory::CLAW) | //
		std::to_underlying(CollisionCategory::PROJECTILE) //
		,

		// 开启的门
		DOOR_OPENED = //
		NONE //
		,

		// 门传感器
		DOOR_SENSOR = //
		std::to_underlying(CollisionCategory::PLAYER) //
		,

		// 玩家
		PLAYER = //
		std::to_underlying(CollisionCategory::WALL) | //
		std::to_underlying(CollisionCategory::DOOR) | //
		std::to_underlying(CollisionCategory::DOOR_SENSOR) | //
		std::to_underlying(CollisionCategory::PLAYER) | //
		std::to_underlying(CollisionCategory::ENEMY) | //
		std::to_underlying(CollisionCategory::CLAW) | //
		std::to_underlying(CollisionCategory::PROJECTILE) //
		,

		// 敌人
		ENEMY = //
		std::to_underlying(CollisionCategory::WALL) | //
		std::to_underlying(CollisionCategory::DOOR) | //
		std::to_underlying(CollisionCategory::PLAYER) | //
		std::to_underlying(CollisionCategory::ENEMY) | //
		std::to_underlying(CollisionCategory::CLAW) | //
		std::to_underlying(CollisionCategory::PROJECTILE) //
		,

		// 爪(近战武器)
		CLAW = //
		std::to_underlying(CollisionCategory::DOOR) | //
		std::to_underlying(CollisionCategory::PLAYER) | //
		std::to_underlying(CollisionCategory::ENEMY) //
		,

		// 飞弹(远程武器)
		PROJECTILE = //
		std::to_underlying(CollisionCategory::WALL) | //
		std::to_underlying(CollisionCategory::DOOR) | //
		std::to_underlying(CollisionCategory::PLAYER) | //
		std::to_underlying(CollisionCategory::ENEMY) //
		,
	};

	// 敌人类型
	enum class EnemyType : std::uint8_t
	{
		// 老鼠
		RAT = 0,
		// 史莱姆
		SLIME,
		// 蝙蝠
		BAT,
	};
}
