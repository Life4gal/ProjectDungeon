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

namespace pd::component::door
{
	// transform --> 门变换
	// collision --> 门碰撞体
	// tags --> 门标签

	// 门形状ID
	class DoorShapeId final
	{
	public:
		b2ShapeId shape_id;
	};

	// 感应区形状ID
	class SensorShapeId final
	{
	public:
		b2ShapeId shape_id;
	};

	// 门所在方向
	using blueprint::Direction;

	// 门状态
	enum class State : std::uint8_t
	{
		// 已关闭
		CLOSED,
		// 已开启
		OPENED,
	};

	// ============================================

	// 门所属房间实体
	// 该组件不由factory::Door附加,而是由factory::Room附加
	class Room final
	{
	public:
		entt::entity room;
	};

	// 门(感应器)所连接的房间实体
	// 该组件不由factory::Door附加,而是由factory::Level附加
	class TargetRoom final
	{
	public:
		entt::entity room;
	};
}
