// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/physics_shape.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::event::door
{
	class ContactBegin final
	{
	public:
		entt::entity door;
		entt::entity other;

		blueprint::PhysicsShapeType other_type;
	};

	class ContactEnd final
	{
	public:
		entt::entity door;
		entt::entity other;

		blueprint::PhysicsShapeType other_type;
	};

	class SensorBegin final
	{
	public:
		entt::entity door;
		entt::entity other;

		blueprint::PhysicsShapeType other_type;
	};

	class SensorEnd final
	{
	public:
		entt::entity door;
		entt::entity other;

		blueprint::PhysicsShapeType other_type;
	};
}
