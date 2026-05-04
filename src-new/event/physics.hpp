// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/physics_shape.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::event::physics
{
	// 任意实体开始接触
	class ContactBegin final
	{
	public:
		entt::entity a;
		entt::entity b;

		blueprint::PhysicsShapeType a_type;
		blueprint::PhysicsShapeType b_type;
	};

	// 任意实体结束接触
	class ContactEnd final
	{
	public:
		entt::entity a;
		entt::entity b;

		blueprint::PhysicsShapeType a_type;
		blueprint::PhysicsShapeType b_type;
	};

	// 任意实体与感应器开始接触
	class SensorBegin final
	{
	public:
		entt::entity sensor;
		entt::entity visitor;

		blueprint::PhysicsShapeType sensor_type;
		blueprint::PhysicsShapeType visitor_type;
	};

	// 任意实体与感应器结束接触
	class SensorEnd final
	{
	public:
		entt::entity sensor;
		entt::entity visitor;

		blueprint::PhysicsShapeType sensor_type;
		blueprint::PhysicsShapeType visitor_type;
	};
}
