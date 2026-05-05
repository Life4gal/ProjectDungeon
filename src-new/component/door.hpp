// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/door.hpp>

#include <component/transform.hpp>
#include <component/sprite.hpp>
#include <component/physics_body.hpp>
#include <component/tags.hpp>

namespace pd::component::door
{
	// transform
	// sprite
	// physics_body
	// tags

	class PhysicsShapeDoor final
	{
	public:
		b2ShapeId shape;
	};

	class PhysicsShapeSensor final
	{
	public:
		b2ShapeId shape;
	};

	class PhysicsShapeBlocker final
	{
	public:
		b2ShapeId shape;
	};

	// blueprint::DoorDirection
	enum class Direction : std::uint8_t
	{
		NORTH = 0b00,
		SOUTH = 0b01,
		WEST = 0b10,
		EAST = 0b11,
	};

	// 门所属房间实体
	// 该组件不由factory::Door附加,而是由factory::Room附加
	class Room final
	{
	public:
		entt::entity room;
	};

	// 门所连接的房间实体
	// 该组件不由factory::Door附加,而是由factory::Level附加
	class TargetRoom final
	{
	public:
		entt::entity target_room;
	};
}
