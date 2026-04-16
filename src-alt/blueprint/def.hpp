// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <cstdint>

namespace pd::blueprint
{
	// ============================================
	// FLOOR
	// ============================================

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

	// ============================================
	// ROOM
	// ============================================

	enum class TileCollision : std::uint8_t
	{
		// 地板
		FLOOR,
		// 墙壁
		WALL,
		// 障碍物
		OBSTACLE,
		// 孔洞
		HOLE,
		// 可破坏物
		DESTROYABLE,
	};

	enum class TileTransform : std::uint8_t
	{
		// 原始朝向
		IDENTITY,
		// 水平翻转
		HORIZONTAL_FLIP,
		// 垂直翻转
		VERTICAL_FLIP,
		// 顺时针旋转90度
		CW_90,
		// 逆时针旋转90度
		CCW_90,
		// 顺时针旋转180度
		ROTATE_180,
		// 转置
		TRANSPOSE,
		// 反转置
		ANTI_TRANSPOSE
	};

	enum class TileOverlayObject : std::uint16_t
	{
		NONE,
	};

	// ============================================
	// DOOR
	// ============================================

	enum class DoorDirection : std::uint8_t
	{
		NORTH = 0b00,
		SOUTH = 0b01,
		WEST = 0b10,
		EAST = 0b11,
	};

	// [[nodiscard]] constexpr auto operator-(const DoorDirection direction) noexcept -> DoorDirection
	// {
	// 	return static_cast<DoorDirection>(std::to_underlying(direction) ^ 0b01);
	// }

	enum class DoorType : std::uint8_t
	{
		// 标准门
		STANDARD,
		// BOSS门
		BOSS,
		// 出口门
		EXIT,
	};
}
