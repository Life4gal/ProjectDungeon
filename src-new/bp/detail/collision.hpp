// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <variant>
#include <vector>

#include <bp/detail/transform.hpp>

namespace pd::bp
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

	// 碰撞体类型
	// b2ShapeDef::filter -> categoryBits
	enum class CollisionCategory : std::uint64_t // NOLINT(performance-enum-size)
	{
		// [0~7]

		WALL = 1u << 0,
		DOOR = 1u << 1,

		// [8~15]

		PLAYER = 1u << 8,
		ENEMY = 1u << 9,

		// [16~23]

		CLAW = 1u << 16,
		PROJECTILE = 1u << 17,

		// [24~31]
		//
	};

	// 碰撞体掩码
	// b2ShapeDef::filter -> maskBits
	enum class CollisionMask : std::underlying_type_t<CollisionCategory> // NOLINT(performance-enum-size)
	{
		NONE = 0,

		WALL = //
		std::to_underlying(CollisionCategory::PLAYER) | //
		std::to_underlying(CollisionCategory::ENEMY) | //
		std::to_underlying(CollisionCategory::PROJECTILE) //
		,

		DOOR = //
		std::to_underlying(CollisionCategory::PLAYER) | //
		std::to_underlying(CollisionCategory::ENEMY) | //
		std::to_underlying(CollisionCategory::PROJECTILE) //
		,

		DOOR_SENSOR = //
		std::to_underlying(CollisionCategory::PLAYER) //
		,

		PLAYER = //
		std::to_underlying(CollisionCategory::WALL) | //
		std::to_underlying(CollisionCategory::DOOR) | //
		std::to_underlying(CollisionCategory::PLAYER) | //
		std::to_underlying(CollisionCategory::ENEMY) | //
		std::to_underlying(CollisionCategory::CLAW) | //
		std::to_underlying(CollisionCategory::PROJECTILE) //
		,

		ENEMY = //
		std::to_underlying(CollisionCategory::WALL) | //
		std::to_underlying(CollisionCategory::DOOR) | //
		std::to_underlying(CollisionCategory::PLAYER) | //
		std::to_underlying(CollisionCategory::ENEMY) | //
		std::to_underlying(CollisionCategory::CLAW) | //
		std::to_underlying(CollisionCategory::PROJECTILE) //
		,

		CLAW = //
		std::to_underlying(CollisionCategory::PLAYER) | //
		std::to_underlying(CollisionCategory::ENEMY) //
		,

		PROJECTILE = //
		std::to_underlying(CollisionCategory::WALL) | //
		std::to_underlying(CollisionCategory::DOOR) | //
		std::to_underlying(CollisionCategory::PLAYER) | //
		std::to_underlying(CollisionCategory::ENEMY) //
		,
	};

	// 圆形形状
	// b2Circle
	class CollisionCircleShape final
	{
	public:
		// 圆心
		Position center;
		// 半径
		float radius;
	};

	// 胶囊形状
	// b2Capsule
	class CollisionCapsuleShape final
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
	class CollisionBoxShape final
	{
	public:
		// 大小
		Size size;
	};

	// 线段形状
	// b2Segment
	class CollisionSegmentShape final
	{
	public:
		// 点1
		Position point1;
		// 点2
		Position point2;
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
	class CollisionShape final : std::variant<
				CollisionCircleShape,
				CollisionCapsuleShape,
				CollisionBoxShape,
				CollisionSegmentShape
			>
	{
	public:
		CollisionShapeDef def;
	};

	// 碰撞体
	class Collision final
	{
	public:
		// 刚体类型
		CollisionBodyType type;

		// 形状
		std::vector<CollisionShape> shapes;
	};
}
