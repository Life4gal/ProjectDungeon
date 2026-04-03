// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/door.hpp>

#include <manager/event.hpp>

#include <game/constants.hpp>

#include <components/tags.hpp>
#include <components/transform.hpp>

#include <systems/physics_world.hpp>
#include <systems/transform.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::systems
{
	namespace
	{
		using namespace components;

		auto create_door_body(entt::registry& registry, entt::entity entity, const sf::Vector2f position, const Door::direction_type direction) noexcept -> void
		{
			const auto world_id = PhysicsWorld::id(registry);

			// ====================
			// 刚体

			auto body_def = b2DefaultBodyDef();
			body_def.type = b2_staticBody;
			body_def.position = PhysicsWorld::physics_position_of(position);
			// body_def.rotation = PhysicsWorld::physics_rotation_of(rotation.rotation);
			body_def.userData = PhysicsWorld::to_user_data(entity);

			const auto body_id = PhysicsWorld::attach(registry, entity, world_id, body_def);

			// ====================
			// 形状

			const auto [width, height] = PhysicsWorld::physics_size_of({game::RoomTileWidth /* * scale.scale.x*/, game::RoomTileHeight /* * scale.scale.y*/});

			const auto is_horizontal = direction == door::Direction::NORTH or direction == door::Direction::SOUTH;
			const auto is_positive = direction == door::Direction::NORTH or direction == door::Direction::EAST;
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

			door::Physics physics{.door = b2_nullShapeId, .sensor = b2_nullShapeId, .blocker = b2_nullShapeId};

			// 门
			{
				auto shape_def = b2DefaultShapeDef();
				shape_def.density = 0;
				shape_def.material.friction = 0;
				// todo
				shape_def.filter = b2DefaultFilter();
				shape_def.filter.categoryBits = 0;
				shape_def.filter.maskBits = 0;
				shape_def.userData = PhysicsWorld::to_user_data(entity);
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

				physics.door = PhysicsWorld::attach_shape(body_id, shape_def, box);
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
				shape_def.userData = PhysicsWorld::to_user_data(entity);
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

				physics.sensor = PhysicsWorld::attach_shape(body_id, shape_def, box);
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
				shape_def.userData = PhysicsWorld::to_user_data(entity);
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

				physics.blocker = PhysicsWorld::attach_shape(body_id, shape_def, box);
			}

			PROMETHEUS_PLATFORM_ASSUME(B2_IS_NON_NULL(physics.door));
			PROMETHEUS_PLATFORM_ASSUME(B2_IS_NON_NULL(physics.sensor));
			PROMETHEUS_PLATFORM_ASSUME(B2_IS_NON_NULL(physics.blocker));

			registry.emplace<door::Physics>(entity, physics);
		}
	}

	auto Door::subscribe_events(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::subscribe<room::PreDisableDoor, &Door::on_pre_disable>(registry);
		Event::subscribe<room::PostDisableDoor, &Door::on_post_disable>(registry);
		Event::subscribe<room::PreEnableDoor, &Door::on_pre_enable>(registry);
		Event::subscribe<room::PostEnableDoor, &Door::on_post_enable>(registry);

		Event::subscribe<door::OpenRequest, &Door::on_open_request>(registry);
		Event::subscribe<door::CloseRequest, &Door::on_close_request>(registry);
		Event::subscribe<door::Contacted, &Door::on_contact_door>(registry);
		Event::subscribe<door::SensorContacted, &Door::on_contact_door_sensor>(registry);
	}

	auto Door::unsubscribe_events(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::unsubscribe<room::PreDisableDoor, &Door::on_pre_disable>(registry);
		Event::unsubscribe<room::PostDisableDoor, &Door::on_post_disable>(registry);
		Event::unsubscribe<room::PreEnableDoor, &Door::on_pre_enable>(registry);
		Event::unsubscribe<room::PostEnableDoor, &Door::on_post_enable>(registry);

		Event::unsubscribe<door::OpenRequest, &Door::on_open_request>(registry);
		Event::unsubscribe<door::CloseRequest, &Door::on_close_request>(registry);
		Event::unsubscribe<door::Contacted, &Door::on_contact_door>(registry);
		Event::unsubscribe<door::SensorContacted, &Door::on_contact_door_sensor>(registry);
	}

	auto Door::on_pre_disable(entt::registry& registry, const events::room::PreDisableDoor& event) noexcept -> void
	{
		for (const auto entity: event.entities)
		{
			// 禁用时销毁物理体
			// FIXME(OPT): 禁用物理体?

			PhysicsWorld::deattach(registry, entity);
		}
	}

	auto Door::on_post_disable([[maybe_unused]] entt::registry& registry, [[maybe_unused]] const events::room::PostDisableDoor& event) noexcept -> void
	{
		//
	}

	auto Door::on_pre_enable(entt::registry& registry, const events::room::PreEnableDoor& event) noexcept -> void
	{
		for (const auto entity: event.entities)
		{
			// 启用时创建物理体
			// FIXME(OPT): 重新启用物理体?

			const auto* position = registry.try_get<transform::Position>(entity);
			if (position == nullptr)
			{
				continue;
			}

			const auto* direction = registry.try_get<door::Direction>(entity);
			if (direction == nullptr)
			{
				continue;
			}

			create_door_body(registry, entity, position->position, *direction);
		}
	}

	auto Door::on_post_enable([[maybe_unused]] entt::registry& registry, [[maybe_unused]] const events::room::PostEnableDoor& event) noexcept -> void
	{
		//
	}

	auto Door::on_open_request(entt::registry& registry, const events::door::OpenRequest& event) noexcept -> void
	{
		for (const auto view = registry.view<door::State>(entt::exclude<tags::disabled>);
		     const auto [entity, state]: view.each())
		{
			if (state == door::State::OPENING or state == door::State::OPENED)
			{
				continue;
			}

			state = door::State::OPENING;
			registry.emplace_or_replace<door::Timer>(entity, sf::Time::Zero);
		}
	}

	auto Door::on_close_request(entt::registry& registry, const events::door::CloseRequest& event) noexcept -> void
	{
		for (const auto view = registry.view<door::State>(entt::exclude<tags::disabled>);
		     const auto [entity, state]: view.each())
		{
			if (state == door::State::CLOSING or state == door::State::CLOSED)
			{
				continue;
			}

			state = door::State::CLOSING;
			registry.emplace_or_replace<door::Timer>(entity, sf::Time::Zero);
		}
	}

	auto Door::on_contact_door([[maybe_unused]] entt::registry& registry, [[maybe_unused]] const events::door::Contacted& event) noexcept -> void
	{
		// 门本身应该是不在乎这个事件的
		// 或者说接触事件不在这里处理
	}

	auto Door::on_contact_door_sensor([[maybe_unused]] entt::registry& registry, [[maybe_unused]] const events::door::SensorContacted& event) noexcept -> void
	{
		// 门本身应该是不在乎这个事件的
		// 或者说接触事件不在这里处理
	}

	auto Door::spawn(entt::registry& registry, const sf::Vector2f position, const direction_type direction, const type_type type) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// door
		{
			create_door_body(registry, entity, position, direction);

			registry.emplace<door::Direction>(entity, direction);
			registry.emplace<door::Type>(entity, type);
			// todo: 锁在哪里设置呢?
			registry.emplace<door::Locked>(entity);
			registry.emplace<door::State>(entity, door::State::CLOSED);
		}
		// transform
		Transform::attach(registry, entity, position);

		registry.emplace<tags::door>(entity);
		return entity;
	}

	auto Door::destroy(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.destroy(entity);
	}

	auto Door::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::door>();
		registry.destroy(view.begin(), view.end());
	}
}
