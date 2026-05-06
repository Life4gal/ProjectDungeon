// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/position.hpp>
#include <blueprint/detail/sprite.hpp>
#include <blueprint/detail/physics_body.hpp>
#include <blueprint/detail/physics_shape.hpp>

namespace pd::blueprint
{
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

	class Door final
	{
	public:
		Position position;
		Sprite sprite;

		// 物理刚体
		PhysicsBody physics_body;
		// 门的碰撞体分为三部分:
		// 1.门: 当且仅当门关闭时存在,或者说当且仅当门关闭时才与玩家碰撞(从而阻止玩家穿过)
		// 2.感应区: 当玩家进入感应区时会将其传送到门对应的房间中
		// 3.阻挡区: 防止玩家意外地穿越感应区进入其本不应该进入的区域
		// 理论上只使用一个PhysicsShapeBox就能描述整个门碰撞体(具体各个部分可以直接计算出来),但是拆分出来可以使结构更清晰,且factory创建时也更方便
		PhysicsShapeOffsetBox physics_shape_door;
		PhysicsShapeOffsetBox physics_shape_sensor;
		PhysicsShapeOffsetBox physics_shape_blocker;

		// 门所在方向
		DoorDirection direction;
	};
}
