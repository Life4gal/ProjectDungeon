// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/process_physics_events.hpp>

#include <utility/physics.hpp>

#include <helper/enemy.hpp>
#include <helper/projectile.hpp>
#include <helper/door.hpp>

#include <prometheus/meta/enumeration.hpp>
#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <box2d/box2d.h>

namespace pd::update
{
	using namespace prometheus;

	namespace
	{
		auto dispatch_contact(
			entt::registry& registry,
			const b2ShapeId a_shape,
			const b2ShapeId b_shape
		) noexcept -> void
		{
			const auto* a_shape_user_data = b2Shape_GetUserData(a_shape);
			const auto* b_shape_user_data = b2Shape_GetUserData(b_shape);
			PROMETHEUS_PLATFORM_ASSUME(a_shape_user_data != nullptr and b_shape_user_data != nullptr);

			const auto a_shape_type = utility::Physics::to_collision_category(a_shape_user_data);
			const auto b_shape_type = utility::Physics::to_collision_category(b_shape_user_data);

			const auto body_a = b2Shape_GetBody(a_shape);
			const auto body_b = b2Shape_GetBody(b_shape);

			const auto* user_data_a = b2Body_GetUserData(body_a);
			const auto* user_data_b = b2Body_GetUserData(body_b);
			PROMETHEUS_PLATFORM_ASSUME(user_data_a != nullptr and user_data_b != nullptr);

			const auto entity_a = utility::Physics::to_entity(user_data_a);
			const auto entity_b = utility::Physics::to_entity(user_data_b);

			// if (not registry.valid(entity_a) or not registry.valid(entity_b))
			// {
			// 	return;
			// }
			PROMETHEUS_PLATFORM_ASSUME(registry.valid(entity_a) and registry.valid(entity_b));

			SPDLOG_INFO(
				"接触事件: [{}]=0x{:08X}, [{}]=0x{:08X}",
				meta::name_of(a_shape_type),
				entt::to_integral(entity_a),
				meta::name_of(b_shape_type),
				entt::to_integral(entity_b)
			);

			// TODO: 接触处理有部分重叠,或者说这之间的判断存在优先级
			//  例如一发飞弹集中一个敌人,是进入敌人接触分支,还是进入飞弹接触分支?

			if (a_shape_type == blueprint::CollisionCategory::PROJECTILE or b_shape_type == blueprint::CollisionCategory::PROJECTILE)
			{
				const auto a = a_shape_type == blueprint::CollisionCategory::PROJECTILE;
				const auto projectile = a ? entity_a : entity_b;
				const auto other = a ? entity_b : entity_a;

				helper::Projectile::contact(registry, projectile, other);
				return;
			}

			if (a_shape_type == blueprint::CollisionCategory::ENEMY or b_shape_type == blueprint::CollisionCategory::ENEMY)
			{
				const auto a = a_shape_type == blueprint::CollisionCategory::ENEMY;
				const auto enemy = a ? entity_a : entity_b;
				const auto other = a ? entity_b : entity_a;

				helper::Enemy::contact(registry, enemy, other);
				return;
			}

			if (a_shape_type == blueprint::CollisionCategory::DOOR or b_shape_type == blueprint::CollisionCategory::DOOR)
			{
				const auto a = a_shape_type == blueprint::CollisionCategory::DOOR;
				const auto door = a ? entity_a : entity_b;
				const auto other = a ? entity_b : entity_a;

				helper::Door::contact(registry, door, other);
				return;
			}

			SPDLOG_WARN("未处理的接触事件!");
		}

		auto process_contact_events(entt::registry& registry, const b2WorldId world_id) noexcept -> void
		{
			const auto contact_events = b2World_GetContactEvents(world_id);

			const auto begin_count = contact_events.beginCount;
			for (int i = 0; i < begin_count; ++i)
			{
				const auto& event = contact_events.beginEvents[i];

				dispatch_contact(registry, event.shapeIdA, event.shapeIdB);
			}
		}

		auto dispatch_sensor(
			entt::registry& registry,
			const b2ShapeId sensor_shape,
			const b2ShapeId visitor_shape
		) noexcept -> void
		{
			const auto* sensor_shape_user_data = b2Shape_GetUserData(sensor_shape);
			const auto* visitor_shape_user_data = b2Shape_GetUserData(visitor_shape);
			PROMETHEUS_PLATFORM_ASSUME(sensor_shape_user_data != nullptr and visitor_shape_user_data != nullptr);

			const auto sensor_shape_type = utility::Physics::to_collision_category(sensor_shape_user_data);
			const auto visitor_shape_type = utility::Physics::to_collision_category(visitor_shape_user_data);

			const auto sensor_body = b2Shape_GetBody(sensor_shape);
			const auto visitor_body = b2Shape_GetBody(visitor_shape);

			const auto* sensor_user_data = b2Body_GetUserData(sensor_body);
			const auto* visitor_user_data = b2Body_GetUserData(visitor_body);
			PROMETHEUS_PLATFORM_ASSUME(sensor_user_data != nullptr and visitor_user_data != nullptr);

			const auto sensor_entity = utility::Physics::to_entity(sensor_user_data);
			const auto visitor_entity = utility::Physics::to_entity(visitor_user_data);

			// if (not registry.valid(sensor_entity) or not registry.valid(visitor_entity))
			// {
			// 	return;
			// }
			PROMETHEUS_PLATFORM_ASSUME(registry.valid(sensor_entity) and registry.valid(visitor_entity));

			SPDLOG_INFO(
				"感应事件: [{}]=0x{:08X}, [{}]=0x{:08X}",
				meta::name_of(visitor_shape_type),
				entt::to_integral(visitor_entity),
				meta::name_of(visitor_shape_type),
				entt::to_integral(visitor_entity)
			);

			if (sensor_shape_type == blueprint::CollisionCategory::DOOR)
			{
				helper::Door::sense(registry, sensor_entity, visitor_entity);
				return;
			}

			SPDLOG_WARN("未处理的感应事件!");
		}

		auto process_sensor_events(entt::registry& registry, const b2WorldId world_id) noexcept -> void
		{
			const auto sensor_events = b2World_GetSensorEvents(world_id);

			const auto begin_count = sensor_events.beginCount;
			for (int i = 0; i < begin_count; ++i)
			{
				const auto& event = sensor_events.beginEvents[i];

				dispatch_sensor(registry, event.sensorShapeId, event.visitorShapeId);
			}
		}
	}

	auto process_physics_events(entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		const auto world_id = utility::Physics::world_id;

		process_contact_events(registry, world_id);
		process_sensor_events(registry, world_id);
	}
}
