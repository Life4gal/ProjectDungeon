// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/def.hpp>

#include <component/transform.hpp>
#include <component/collision.hpp>
#include <component/tags.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::component::door_sensor
{
	// transform --> 门感应器变换
	// collision --> 门感应器碰撞体
	// tags --> 门感应器标签

	// 门感应器形状
	class ShapeId final
	{
	public:
		b2ShapeId shape;
	};

	// 门感应器所在方向
	using blueprint::Direction;

	// ============================================

	// 门感应器所属房间实体
	// 该组件不由factory::DoorSensor附加,而是由factory::Room附加
	class Room final
	{
	public:
		entt::entity room;
	};

	// 门感应器所连接的房间实体
	// 该组件不由factory::DoorSensor附加,而是由factory::Level附加
	class TargetRoom final
	{
	public:
		entt::entity room;
	};
}
