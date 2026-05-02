// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/physics_shape.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::event::door
{
	// 任意实体与门接触
	class ContactBegin final
	{
	public:
		entt::entity door;
		entt::entity other;

		blueprint::PhysicsShapeType other_type;
	};

	// 任意实体与门接触
	class ContactEnd final
	{
	public:
		entt::entity door;
		entt::entity other;

		blueprint::PhysicsShapeType other_type;
	};

	// 任意实体与感应器接触
	class SensorBegin final
	{
	public:
		entt::entity door;
		entt::entity other;

		blueprint::PhysicsShapeType other_type;
	};

	// 任意实体与感应器接触
	class SensorEnd final
	{
	public:
		entt::entity door;
		entt::entity other;

		blueprint::PhysicsShapeType other_type;
	};

	// 请求开启所有门(如果其无需钥匙)
	// 需要钥匙的门在ContactBegin中处理
	class RequestOpen final {};

	// 请求关闭所有门
	class RequestClose final {};
}
