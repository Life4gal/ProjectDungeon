// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/update/process_contact_events.hpp>

#include <components/tags.hpp>
#include <components/name.hpp>

#include <systems/helper/physics_world.hpp>
#include <systems/helper/dungeon.hpp>

#include <prometheus/platform/os.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <box2d/box2d.h>

namespace pd::systems::update
{
	namespace
	{
		auto process_contact_events(entt::registry& registry, const b2WorldId world_id) noexcept -> void
		{
			using namespace components;
			using namespace systems;

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

					const auto entity_a = helper::PhysicsWorld::to_entity(user_data_a);
					const auto entity_b = helper::PhysicsWorld::to_entity(user_data_b);

					const auto& [name_a] = registry.get<name::Name>(entity_a);
					const auto& [name_b] = registry.get<name::Name>(entity_b);

					SPDLOG_INFO("ContactBegin: {} & {}", name_a, name_b);

					// trigger
					{
						const auto is_trigger_a = registry.all_of<tags::trigger>(entity_a);
						const auto is_trigger_b = registry.all_of<tags::trigger>(entity_b);
						if (is_trigger_a or is_trigger_b)
						{
							helper::Dungeon::on_trigger_contact(registry, is_trigger_a ? entity_a : entity_b, is_trigger_a ? entity_b : entity_a);
							continue;
						}
					}
					// key
					{
						const auto is_key_a = registry.all_of<tags::key>(entity_a);
						const auto is_key_b = registry.all_of<tags::key>(entity_b);
						if (is_key_a or is_key_b)
						{
							helper::Dungeon::on_key_contact(registry, is_key_a ? entity_a : entity_b, is_key_a ? entity_b : entity_a);
							continue;
						}
					}
					// door
					{
						const auto is_door_a = registry.all_of<tags::door>(entity_a);
						const auto is_door_b = registry.all_of<tags::door>(entity_b);
						if (is_door_a or is_door_b)
						{
							helper::Dungeon::on_locked_door_contact(registry, is_door_a ? entity_a : entity_b, is_door_a ? entity_b : entity_a);
							continue;
						}
					}
				}
			}

			// end
			{
				const auto end_count = contact_events.endCount;
				for (int i = 0; i < end_count; ++i)
				{
					const auto& event = contact_events.endEvents[i];

					const auto body_a = b2Shape_GetBody(event.shapeIdA);
					const auto body_b = b2Shape_GetBody(event.shapeIdB);

					const auto* user_data_a = b2Body_GetUserData(body_a);
					const auto* user_data_b = b2Body_GetUserData(body_b);

					PROMETHEUS_PLATFORM_ASSUME(user_data_a != nullptr and user_data_b != nullptr);

					const auto entity_a = helper::PhysicsWorld::to_entity(user_data_a);
					const auto entity_b = helper::PhysicsWorld::to_entity(user_data_b);

					const auto& [name_a] = registry.get<name::Name>(entity_a);
					const auto& [name_b] = registry.get<name::Name>(entity_b);

					SPDLOG_INFO("ContactEnd: {} & {}", name_a, name_b);

					//
				}
			}
		}

		auto process_sensor_events(entt::registry& registry, const b2WorldId world_id) noexcept -> void
		{
			using namespace components;
			using namespace systems;

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

					const auto sensor_entity = helper::PhysicsWorld::to_entity(sensor_user_data);
					const auto visitor_entity = helper::PhysicsWorld::to_entity(visitor_user_data);

					const auto& [sensor_name] = registry.get<name::Name>(sensor_entity);
					const auto& [visitor_name] = registry.get<name::Name>(visitor_entity);

					SPDLOG_INFO("SensorBegin: {} & {}", sensor_name, visitor_name);

					// 处理玩家进入门
					if (registry.all_of<tags::door>(sensor_entity))
					{
						// 只有打开的门才是传感器
						helper::Dungeon::on_unlocked_door_contact(registry, sensor_entity, visitor_entity);
					}

					// todo: 处理敌人/玩家接触地板装饰物
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

					const auto sensor_entity = helper::PhysicsWorld::to_entity(sensor_user_data);
					const auto visitor_entity = helper::PhysicsWorld::to_entity(visitor_user_data);

					const auto& [sensor_name] = registry.get<name::Name>(sensor_entity);
					const auto& [visitor_name] = registry.get<name::Name>(visitor_entity);

					SPDLOG_INFO("SensorEnd: {} & {}", sensor_name, visitor_name);

					//
				}
			}
		}
	}

	auto process_contact_events(entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		using namespace systems;

		const auto world_id = helper::PhysicsWorld::id(registry);

		process_contact_events(registry, world_id);
		process_sensor_events(registry, world_id);
	}
}
