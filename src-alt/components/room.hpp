// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <utility/fixed_matrix.hpp>

#include <game/room.hpp>

namespace pd::components::room
{
	enum class CollisionLogical : std::uint8_t
	{
		// 地板
		FLOOR,
		// 墙壁
		WALL,
		// 障碍物
		OBSTACLE,
		// 洞
		HOLE,
		// 可摧毁物
		DESTROYABLE,
	};

	enum class Transform : std::uint8_t
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

	enum class DoorState : std::uint8_t
	{
		// 已关闭
		CLOSED,
		// 正在关闭
		CLOSING,
		// 已开启
		OPENED,
		// 正在开启
		OPENING,
	};

	enum class DoorType : std::uint8_t
	{
		// 标准门
		STANDARD,
		// BOSS门
		BOSS,
		// 出口门
		EXIT,
	};

	class Tile final
	{
	public:
		// todo: 精灵图索引?

		CollisionLogical collision_logical;
		Transform transform;
	};

	// ==========================================
	// 上下文组件
	// ==========================================

	class Room;

	class CurrentRoom final
	{
	public:
		std::reference_wrapper<Room> room;
	};

	// ==========================================
	// 实体组件
	// ==========================================

	// FIXME: 当前房间的门通过registry.view<tags::door, ...>(entt::exclude<tags::disabled>)来访问,要不要直接放到上下文组件中?
	class Door final
	{
	public:
		// 方向
		game::DoorDirection direction;

		// 状态
		DoorState state;
		// 类型
		DoorType type;
		// 动画计时(毫秒)
		std::uint8_t timer_ms;
	};

	// 含有该标签的门无法被systems::Room::open_doors打开
	class LockedDoor {};

	// 门感应器
	class DoorSensor final
	{
	public:
		// todo
	};

	class Room final
	{
	public:
		using size_type = game::Room::size_type;

		utility::FixedMatrix<Tile, game::Room::grid_width, game::Room::grid_height> grid;
	};
}
