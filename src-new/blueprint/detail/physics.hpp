// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <variant>
#include <type_traits>

#include <blueprint/detail/transform.hpp>

namespace pd::blueprint
{
	enum class BodyType : std::uint8_t
	{
		// b2_staticBody
		STATIC = 0,

		// b2_kinematicBody
		KINEMATIC,

		// b2_dynamicBody
		DYNAMIC,
	};

	enum class ShapeType : std::uint32_t
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

	class CollisionMask final
	{
	public:
		using value_type = std::underlying_type_t<ShapeType>;

	private:
		constexpr static value_type placeholder = 0;

	public:
		constexpr static value_type floor = //
				placeholder //
		;

		constexpr static value_type wall = //
				std::to_underlying(ShapeType::PLAYER) | //
				std::to_underlying(ShapeType::ENEMY) | //
				std::to_underlying(ShapeType::PROJECTILE) | //
				placeholder //
		;

		constexpr static value_type door_close = //
				std::to_underlying(ShapeType::PLAYER) | //
				std::to_underlying(ShapeType::ENEMY) | //
				std::to_underlying(ShapeType::PROJECTILE) | //
				placeholder //
		;

		constexpr static value_type door_open = //
				placeholder //
		;

		constexpr static value_type door_sensor = //
				std::to_underlying(ShapeType::PLAYER) | //
				placeholder //
		;

		constexpr static value_type door_blocker = //
				std::to_underlying(ShapeType::PLAYER) | //
				placeholder //
		;

		constexpr static value_type player = //
				std::to_underlying(ShapeType::WALL) | //
				std::to_underlying(ShapeType::DOOR) | //
				std::to_underlying(ShapeType::PLAYER) | //
				std::to_underlying(ShapeType::ENEMY) | //
				std::to_underlying(ShapeType::CLAW) | //
				std::to_underlying(ShapeType::PROJECTILE) | //
				placeholder //
		;

		constexpr static value_type enemy = //
				std::to_underlying(ShapeType::WALL) | //
				std::to_underlying(ShapeType::DOOR) | //
				std::to_underlying(ShapeType::PLAYER) | //
				std::to_underlying(ShapeType::ENEMY) | //
				std::to_underlying(ShapeType::CLAW) | //
				std::to_underlying(ShapeType::PROJECTILE) | //
				placeholder //
		;

		constexpr static value_type claw = //
				std::to_underlying(ShapeType::PLAYER) | //
				std::to_underlying(ShapeType::ENEMY) | //
				placeholder //
		;

		constexpr static value_type projectile = //
				std::to_underlying(ShapeType::WALL) | //
				std::to_underlying(ShapeType::DOOR) | //
				std::to_underlying(ShapeType::PLAYER) | //
				std::to_underlying(ShapeType::ENEMY) | //
				placeholder //
		;
	};

	class BodyDesc final
	{
	public:
		BodyType type;

		// b2BodyDef::fixedRotation
		bool fixed_rotation;

		// b2BodyDef::isBullet
		bool is_bullet;

		// TODO: 其他需要的信息?
	};

	class ShapeDesc final
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
		ShapeType category;
		CollisionMask::value_type category_mask;

		// b2ShapeDef::isSensor
		bool is_sensor;

		// b2ShapeDef::enableContactEvents
		bool enable_sensor_events;

		// b2ShapeDef::enableContactEvents
		bool enable_contact_events;
	};

	class ShapeCategory final
	{
	public:
		class Circle final
		{
		public:
			// 半径
			float radius;
		};

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

		class Box final
		{
		public:
			// 大小
			Size size;
		};

		class OffsetBox final
		{
		public:
			// 大小
			Size size;
			// 偏移
			Position offset;
			// 旋转
			Rotation rotation;
		};

		class Any final : public std::variant<Circle, Capsule, Box, OffsetBox> {};
	};
}
