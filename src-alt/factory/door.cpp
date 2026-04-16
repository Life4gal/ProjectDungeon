// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/door.hpp>

#include <components/tags.hpp>
#include <components/room.hpp>

#include <accessor/transform.hpp>
#include <accessor/physics.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::factory
{
	namespace crd = components::room::door;

	namespace
	{
		auto create_physics_body(entt::registry& registry, entt::entity entity, sf::Vector2f position, Door::direction_type direction) noexcept -> void
		{
			// ====================
			// 刚体

			auto body_def = b2DefaultBodyDef();
			body_def.type = b2_staticBody;
			body_def.position = accessor::Physics::physics_position_of(position);
			// body_def.rotation = PhysicsWorld::physics_rotation_of(rotation.rotation);
			body_def.userData = accessor::Physics::to_user_data(entity);

			auto shape_attacher = accessor::Physics::attach(registry, entity, body_def);

			// ====================
			// 形状

			const auto [width, height] = accessor::Physics::physics_size_of({game::RoomTileWidth /* * scale.scale.x*/, game::RoomTileHeight /* * scale.scale.y*/});

			const auto is_horizontal = direction == Door::direction_type::NORTH or direction == Door::direction_type::SOUTH;
			const auto is_positive = direction == Door::direction_type::NORTH or direction == Door::direction_type::EAST;
			const auto sign = is_positive ? 1.0f : -1.0f;

			const auto total_length = is_horizontal ? width : height;
			const auto perpendicular_size = is_horizontal ? height : width;
			const auto half_total_length = total_length * 0.5f;
			const auto halt_perpendicular_size = perpendicular_size * 0.5f;

			// 各个形状宽度/高度
			const auto door_size = total_length * game::DoorBodyRatio;
			const auto sensor_size = total_length * game::DoorSensorRatio;
			const auto blocker_size = total_length * game::DoorBlockerRatio;

			// 各个形状偏移
			const auto door_offset = door_size * 0.5f * sign;
			const auto sensor_offset = (door_size + sensor_size) * 0.5f * sign;
			const auto blocker_offset = (door_size + sensor_size + blocker_size) * 0.5f * sign;

			// 各个形状位置
			const auto calculate_position = [&](const auto offset) noexcept -> b2Vec2
			{
				if (is_horizontal)
				{
					return {position.x + offset, position.y};
				}

				return {position.x, position.y + offset};
			};

			const auto door_position = calculate_position(-half_total_length + door_offset);
			const auto sensor_position = calculate_position(-half_total_length + sensor_offset);
			const auto blocker_position = calculate_position(-half_total_length + blocker_offset);

			crd::Physics physics{.door = b2_nullShapeId, .sensor = b2_nullShapeId, .blocker = b2_nullShapeId};

			// 门
			{
				auto shape_def = b2DefaultShapeDef();
				shape_def.density = 0;
				shape_def.material.friction = 0;
				// todo
				shape_def.filter = b2DefaultFilter();
				shape_def.filter.categoryBits = 0;
				shape_def.filter.maskBits = 0;
				shape_def.userData = accessor::Physics::to_user_data(entity);
				shape_def.isSensor = false;
				shape_def.enableContactEvents = true;

				const auto box = [&] noexcept -> b2Polygon
				{
					if (is_horizontal)
					{
						return b2MakeOffsetBox(door_size * 0.5f, halt_perpendicular_size, door_position, b2Rot_identity);
					}

					return b2MakeOffsetBox(halt_perpendicular_size, door_size * 0.5f, door_position, b2Rot_identity);
				}();

				physics.door = shape_attacher.attach(shape_def, box);
			}
			// 感应区
			{
				auto shape_def = b2DefaultShapeDef();
				shape_def.density = 0;
				shape_def.material.friction = 0;
				// todo
				shape_def.filter = b2DefaultFilter();
				shape_def.filter.categoryBits = 0;
				shape_def.filter.maskBits = 0;
				shape_def.userData = accessor::Physics::to_user_data(entity);
				shape_def.isSensor = true;
				shape_def.enableContactEvents = true;

				const auto box = [&] noexcept -> b2Polygon
				{
					if (is_horizontal)
					{
						return b2MakeOffsetBox(sensor_size * 0.5f, halt_perpendicular_size, sensor_position, b2Rot_identity);
					}
					return b2MakeOffsetBox(halt_perpendicular_size, sensor_size * 0.5f, sensor_position, b2Rot_identity);
				}();

				physics.sensor = shape_attacher.attach(shape_def, box);
			}
			// 阻挡
			{
				auto shape_def = b2DefaultShapeDef();
				shape_def.density = 0;
				shape_def.material.friction = 0;
				// todo
				shape_def.filter = b2DefaultFilter();
				shape_def.filter.categoryBits = 0;
				shape_def.filter.maskBits = 0;
				shape_def.userData = accessor::Physics::to_user_data(entity);
				shape_def.isSensor = false;
				shape_def.enableContactEvents = false;

				const auto box = [&] noexcept -> b2Polygon
				{
					if (is_horizontal)
					{
						return b2MakeOffsetBox(blocker_size * 0.5f, halt_perpendicular_size, blocker_position, b2Rot_identity);
					}
					return b2MakeOffsetBox(halt_perpendicular_size, blocker_size * 0.5f, blocker_position, b2Rot_identity);
				}();

				physics.blocker = shape_attacher.attach(shape_def, box);
			}

			PROMETHEUS_PLATFORM_ASSUME(B2_IS_NON_NULL(physics.door));
			PROMETHEUS_PLATFORM_ASSUME(B2_IS_NON_NULL(physics.sensor));
			PROMETHEUS_PLATFORM_ASSUME(B2_IS_NON_NULL(physics.blocker));

			registry.emplace<crd::Physics>(entity, physics);
		}
	}

	auto Door::spawn(entt::registry& registry, const sf::Vector2f position, const direction_type direction, const type_type type) noexcept -> entt::entity
	{
		using namespace components;

		const auto entity = registry.create();

		// door
		{
			create_physics_body(registry, entity, position, direction);

			registry.emplace<crd::Direction>(entity, direction);
			registry.emplace<crd::Type>(entity, type);
			// todo: 锁在哪里设置呢?
			registry.emplace<crd::Locked>(entity);
			registry.emplace<crd::State>(entity, crd::State::CLOSED);
		}
		// transform
		accessor::Transform::attach(registry, entity, position);

		registry.emplace<tags::door>(entity);
		return entity;
	}

	auto Door::destroy(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.destroy(entity);
	}

	auto Door::destroy_all(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		const auto view = registry.view<tags::door>();
		registry.destroy(view.begin(), view.end());
	}
}
