// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <variant>
#include <vector>

#include <blueprint/detail/transform.hpp>
#include <blueprint/def.hpp>

namespace pd::blueprint
{
	// 碰撞体刚体类型
	enum class CollisionBodyType : std::uint8_t
	{
		// b2_staticBody
		STATIC = 0,

		// b2_kinematicBody
		KINEMATIC,

		// b2_dynamicBody
		DYNAMIC,
	};

	class CollisionBodyDef final
	{
	public:
		// 刚体类型
		CollisionBodyType type;
		// 是否允许旋转
		// b2BodyDef::fixedRotation
		bool fixed_rotation;
		// 是否是子弹
		// b2BodyDef::isBullet
		bool is_bullet;

		// TODO: 其他需要的信息?
	};

	// 形状定义
	// b2ShapeDef
	class CollisionShapeDef final
	{
	public:
		class Material final
		{
		public:
			// b2SurfaceMaterial::friction
			float friction;

			// b2SurfaceMaterial::restitution
			float restitution;
		};

		// b2ShapeDef::material
		Material material;

		// b2ShapeDef::density
		float density;

		// b2ShapeDef::filter
		CollisionCategory category;
		CollisionMask mask;

		// b2ShapeDef::isSensor
		bool is_sensor;

		// b2ShapeDef::enableContactEvents
		bool enable_sensor_events;

		// b2ShapeDef::enableContactEvents
		bool enable_contact_events;
	};

	// 碰撞体形状
	class CollisionShape final
	{
	public:
		// 圆形形状
		// b2Circle
		class Circle final
		{
		public:
			// 圆心
			Position center;
			// 半径
			float radius;
		};

		// 胶囊形状
		// b2Capsule
		class Capsule final
		{
		public:
			// 原点1
			Position center1;
			// 原点2
			Position center2;
			// 半径
			float radius;
		};

		// 矩形形状
		// b2Polygon
		class Box final
		{
		public:
			// 中心点 == 刚体原点
			// 大小
			Size size;
		};

		// 矩形形状
		// b2Polygon
		class OffsetBox final
		{
		public:
			// 中心点
			Position center;
			// 大小
			Size size;
			// 旋转
			Rotation rotation;
		};

		// 线段形状
		// b2Segment
		class Segment final
		{
		public:
			// 点1
			Position point1;
			// 点2
			Position point2;
		};

		using shape_type = std::variant<
			Circle,
			Capsule,
			Box,
			OffsetBox,
			Segment
		>;

		CollisionShapeDef def;
		shape_type shape;
	};

	// 碰撞体
	class Collision final
	{
	public:
		CollisionBodyDef def;

		// 形状
		std::vector<CollisionShape> shapes;
	};
}
