// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

namespace pd::blueprint
{
	class PhysicsShapeMaterial final
	{
	public:
		// b2SurfaceMaterial::friction
		float friction;

		// b2SurfaceMaterial::restitution
		float restitution;
	};

	class PhysicsShapeDef final
	{
	public:
		PhysicsShapeMaterial material;

		// b2ShapeDef::isSensor
		bool is_sensor;

		// b2ShapeDef::enableContactEvents
		bool enable_sensor_events;

		// b2ShapeDef::enableContactEvents
		bool enable_contact_events;
	};

	class PhysicsShapeCircle final
	{
	public:
		PhysicsShapeDef def;

		float radius;
	};

	class PhysicsShapeCapsule final
	{
	public:
		PhysicsShapeDef def;

		float center1_offset_x;
		float center1_offset_y;

		float center2_offset_x;
		float center2_offset_y;

		float radius;
	};

	class PhysicsShapeBox final
	{
	public:
		PhysicsShapeDef def;

		float width;
		float height;
	};

	class PhysicsShapeOffsetBox final
	{
	public:
		PhysicsShapeDef def;

		float width;
		float height;

		float offset_x;
		float offset_y;
	};
}
