// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/process_physics_events.hpp>

#include <utility/physics.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

namespace pd::update
{
	namespace
	{
		auto process_contact_events(entt::registry& registry, const b2WorldId world_id) noexcept -> void
		{
			const auto contact_events = b2World_GetContactEvents(world_id);

			// begin
			{
				const auto begin_count = contact_events.beginCount;
				for (int i = 0; i < begin_count; ++i)
				{
					const auto& event = contact_events.beginEvents[i];

					const auto body_a = b2Shape_GetBody(event.shapeIdA);
					const auto body_b = b2Shape_GetBody(event.shapeIdB);

					const auto* user_data_a = b2Body_GetUserData(body_a);
					const auto* user_data_b = b2Body_GetUserData(body_b);

					PROMETHEUS_PLATFORM_ASSUME(user_data_a != nullptr and user_data_b != nullptr);

					const auto entity_a = utility::Physics::to_entity(user_data_a);
					const auto entity_b = utility::Physics::to_entity(user_data_b);

					if (not registry.valid(entity_a) or not registry.valid(entity_b))
					{
						continue;
					}

					SPDLOG_INFO("Contact began between entities 0x{:08X} and 0x{:08X}", entt::to_integral(entity_a), entt::to_integral(entity_b));
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

					const auto body_a = b2Shape_GetBody(event.shapeIdA);
					const auto body_b = b2Shape_GetBody(event.shapeIdB);

					const auto* user_data_a = b2Body_GetUserData(body_a);
					const auto* user_data_b = b2Body_GetUserData(body_b);

					PROMETHEUS_PLATFORM_ASSUME(user_data_a != nullptr and user_data_b != nullptr);

					const auto entity_a = utility::Physics::to_entity(user_data_a);
					const auto entity_b = utility::Physics::to_entity(user_data_b);

					if (not registry.valid(entity_a) or not registry.valid(entity_b))
					{
						continue;
					}

					SPDLOG_INFO("Contact ended between entities 0x{:08X} and 0x{:08X}", entt::to_integral(entity_a), entt::to_integral(entity_b));
				}
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

					const auto sensor_body = b2Shape_GetBody(event.sensorShapeId);
					const auto visitor_body = b2Shape_GetBody(event.visitorShapeId);

					const auto* sensor_user_data = b2Body_GetUserData(sensor_body);
					const auto* visitor_user_data = b2Body_GetUserData(visitor_body);

					PROMETHEUS_PLATFORM_ASSUME(sensor_user_data != nullptr and visitor_user_data != nullptr);

					const auto sensor_entity = utility::Physics::to_entity(sensor_user_data);
					const auto visitor_entity = utility::Physics::to_entity(visitor_user_data);

					if (not registry.valid(sensor_entity) or not registry.valid(visitor_entity))
					{
						continue;
					}

					SPDLOG_INFO("Sensor began between entities 0x{:08X} and 0x{:08X}", entt::to_integral(sensor_entity), entt::to_integral(visitor_entity));
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

					const auto sensor_body = b2Shape_GetBody(event.sensorShapeId);
					const auto visitor_body = b2Shape_GetBody(event.visitorShapeId);

					const auto* sensor_user_data = b2Body_GetUserData(sensor_body);
					const auto* visitor_user_data = b2Body_GetUserData(visitor_body);

					PROMETHEUS_PLATFORM_ASSUME(sensor_user_data != nullptr and visitor_user_data != nullptr);

					const auto sensor_entity = utility::Physics::to_entity(sensor_user_data);
					const auto visitor_entity = utility::Physics::to_entity(visitor_user_data);

					if (not registry.valid(sensor_entity) or not registry.valid(visitor_entity))
					{
						continue;
					}

					SPDLOG_INFO("Contact ended between entities 0x{:08X} and 0x{:08X}", entt::to_integral(sensor_entity), entt::to_integral(visitor_entity));
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
