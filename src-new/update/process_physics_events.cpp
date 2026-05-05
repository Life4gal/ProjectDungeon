// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/process_physics_events.hpp>

#include <utility/physics.hpp>

#include <manager/event.hpp>

#include <event/physics.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::update
{
	namespace
	{
		enum class Type : std::uint8_t
		{
			BEGIN,
			END,
		};

		template<Type T>
		auto dispatch_contact(
			entt::registry& registry,
			const b2ShapeId a_shape,
			const b2ShapeId b_shape
		) noexcept -> void
		{
			const auto* a_shape_user_data = b2Shape_GetUserData(a_shape);
			const auto* b_shape_user_data = b2Shape_GetUserData(b_shape);
			PROMETHEUS_PLATFORM_ASSUME(a_shape_user_data != nullptr and b_shape_user_data != nullptr);

			const auto a_shape_type = utility::Physics::to_shape_type(a_shape_user_data);
			const auto b_shape_type = utility::Physics::to_shape_type(b_shape_user_data);

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

			if constexpr (T == Type::BEGIN)
			{
				manager::Event::enqueue(event::physics::ContactBegin{.a = entity_a, .b = entity_b, .a_type = a_shape_type, .b_type = b_shape_type});
			}
			else
			{
				manager::Event::enqueue(event::physics::ContactEnd{.a = entity_a, .b = entity_b, .a_type = a_shape_type, .b_type = b_shape_type});
			}
		}

		auto process_contact_events(entt::registry& registry, const b2WorldId world_id) noexcept -> void
		{
			const auto contact_events = b2World_GetContactEvents(world_id);

			// begin
			{
				const auto begin_count = contact_events.beginCount;
				for (int i = 0; i < begin_count; ++i)
				{
					const auto& event = contact_events.beginEvents[i];

					dispatch_contact<Type::BEGIN>(registry, event.shapeIdA, event.shapeIdB);
				}
			}

			// end
			{
				const auto end_count = contact_events.endCount;
				for (int i = 0; i < end_count; ++i)
				{
					const auto& event = contact_events.endEvents[i];

					if (not b2Shape_IsValid(event.shapeIdA) or not b2Shape_IsValid(event.shapeIdB))
					{
						continue;
					}

					dispatch_contact<Type::END>(registry, event.shapeIdA, event.shapeIdB);
				}
			}
		}

		template<Type T>
		auto dispatch_sensor(
			entt::registry& registry,
			const b2ShapeId sensor_shape,
			const b2ShapeId visitor_shape
		) noexcept -> void
		{
			const auto* sensor_shape_user_data = b2Shape_GetUserData(sensor_shape);
			const auto* visitor_shape_user_data = b2Shape_GetUserData(visitor_shape);
			PROMETHEUS_PLATFORM_ASSUME(sensor_shape_user_data != nullptr and visitor_shape_user_data != nullptr);

			const auto sensor_shape_type = utility::Physics::to_shape_type(sensor_shape_user_data);
			const auto visitor_shape_type = utility::Physics::to_shape_type(visitor_shape_user_data);

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

			if constexpr (T == Type::BEGIN)
			{
				manager::Event::enqueue(event::physics::SensorBegin{.sensor = sensor_entity, .visitor = visitor_entity, .sensor_type = sensor_shape_type, .visitor_type = visitor_shape_type});
			}
			else
			{
				manager::Event::enqueue(event::physics::SensorEnd{.sensor = sensor_entity, .visitor = visitor_entity, .sensor_type = sensor_shape_type, .visitor_type = visitor_shape_type});
			}
		}

		auto process_sensor_events(entt::registry& registry, const b2WorldId world_id) noexcept -> void
		{
			const auto sensor_events = b2World_GetSensorEvents(world_id);

			// begin
			{
				const auto begin_count = sensor_events.beginCount;
				for (int i = 0; i < begin_count; ++i)
				{
					const auto& event = sensor_events.beginEvents[i];

					dispatch_sensor<Type::BEGIN>(registry, event.sensorShapeId, event.visitorShapeId);
				}
			}

			// end
			{
				const auto end_count = sensor_events.endCount;
				for (int i = 0; i < end_count; ++i)
				{
					const auto& event = sensor_events.endEvents[i];

					if (not b2Shape_IsValid(event.sensorShapeId) or not b2Shape_IsValid(event.visitorShapeId))
					{
						continue;
					}

					dispatch_sensor<Type::END>(registry, event.sensorShapeId, event.visitorShapeId);
				}
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
