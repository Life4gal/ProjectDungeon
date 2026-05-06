// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <utility>
#include <type_traits>

#include <blueprint/detail/position.hpp>
#include <blueprint/detail/size.hpp>

namespace pd::blueprint
{
	enum class PhysicsShapeType : std::uint32_t
	{
		// [0~7]
		FLOOR = 1u << 0,
		WALL = 1u << 1,
		DOOR = 1u << 2,

		// [8~15]

		PLAYER = 1u << 8,
		ENEMY = 1u << 9,

		// [16~23]

		CLAW = 1u << 16,
		PROJECTILE = 1u << 17,

		// [24~31]
		//
	};

	class PhysicsShapeCollisionMask final
	{
	public:
		using value_type = std::underlying_type_t<PhysicsShapeType>;

	private:
		constexpr static value_type placeholder = 0;

	public:
		constexpr static value_type floor = //
				placeholder //
		;

		constexpr static value_type wall = //
				std::to_underlying(PhysicsShapeType::PLAYER) | //
				std::to_underlying(PhysicsShapeType::ENEMY) | //
				std::to_underlying(PhysicsShapeType::PROJECTILE) | //
				placeholder //
		;

		constexpr static value_type door_close = //
				std::to_underlying(PhysicsShapeType::PLAYER) | //
				std::to_underlying(PhysicsShapeType::ENEMY) | //
				std::to_underlying(PhysicsShapeType::PROJECTILE) | //
				placeholder //
		;

		constexpr static value_type door_open = //
				placeholder //
		;

		constexpr static value_type door_sensor = //
				std::to_underlying(PhysicsShapeType::PLAYER) | //
				placeholder //
		;

		constexpr static value_type door_blocker = //
				std::to_underlying(PhysicsShapeType::PLAYER) | //
				placeholder //
		;

		constexpr static value_type player = //
				std::to_underlying(PhysicsShapeType::WALL) | //
				std::to_underlying(PhysicsShapeType::DOOR) | //
				std::to_underlying(PhysicsShapeType::PLAYER) | //
				std::to_underlying(PhysicsShapeType::ENEMY) | //
				std::to_underlying(PhysicsShapeType::CLAW) | //
				std::to_underlying(PhysicsShapeType::PROJECTILE) | //
				placeholder //
		;

		constexpr static value_type enemy = //
				std::to_underlying(PhysicsShapeType::WALL) | //
				std::to_underlying(PhysicsShapeType::DOOR) | //
				std::to_underlying(PhysicsShapeType::PLAYER) | //
				std::to_underlying(PhysicsShapeType::ENEMY) | //
				std::to_underlying(PhysicsShapeType::CLAW) | //
				std::to_underlying(PhysicsShapeType::PROJECTILE) | //
				placeholder //
		;

		constexpr static value_type claw = //
				std::to_underlying(PhysicsShapeType::PLAYER) | //
				std::to_underlying(PhysicsShapeType::ENEMY) | //
				placeholder //
		;

		constexpr static value_type projectile = //
				std::to_underlying(PhysicsShapeType::WALL) | //
				std::to_underlying(PhysicsShapeType::DOOR) | //
				std::to_underlying(PhysicsShapeType::PLAYER) | //
				std::to_underlying(PhysicsShapeType::ENEMY) | //
				placeholder //
		;
	};

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
		// b2ShapeDef::material
		PhysicsShapeMaterial material;

		// b2ShapeDef::density
		float density;

		// b2ShapeDef::filter
		PhysicsShapeType category;
		PhysicsShapeCollisionMask::value_type category_mask;

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

		// 半径
		float radius;
	};

	class PhysicsShapeCapsule final
	{
	public:
		PhysicsShapeDef def;

		// 原点1
		Position center1;
		// 原点2
		Position center2;
		// 半径
		float radius;
	};

	class PhysicsShapeBox final
	{
	public:
		PhysicsShapeDef def;

		// 大小
		Size size;
	};

	class PhysicsShapeOffsetBox final
	{
	public:
		PhysicsShapeDef def;

		// 大小
		Size size;
		// 偏移
		Position offset;
		// 旋转(角度)
		float rotation;
	};
}
