// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/door.hpp>

#include <component/door.hpp>

#include <assembly/transform.hpp>
#include <assembly/render.hpp>
#include <assembly/collision.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::factory
{
	using namespace component;

	namespace
	{
		constexpr blueprint::CollisionBodyDef BodyDef
		{
				.type = blueprint::CollisionBodyType::STATIC,
				.fixed_rotation = true,
				.is_bullet = false,
		};
		constexpr blueprint::CollisionShapeDef DoorShapeDef
		{
				.material = {.friction = 0.3f, .restitution = 0},
				.density = 0,
				.category = blueprint::CollisionCategory::DOOR,
				.mask = blueprint::CollisionMask::DOOR,
				.is_sensor = false,
				.enable_sensor_events = false,
				// 允许接触事件(例如提醒玩家需要钥匙才能打开?)
				.enable_contact_events = true,
		};
		constexpr blueprint::CollisionShapeDef SensorShapeDef
		{
				.material = {.friction = 0.3f, .restitution = 0},
				.density = 0,
				.category = blueprint::CollisionCategory::DOOR_SENSOR,
				.mask = blueprint::CollisionMask::DOOR_SENSOR,
				.is_sensor = true,
				.enable_sensor_events = true,
				.enable_contact_events = false,
		};
	}

	auto Door::spawn(entt::registry& registry, const blueprint::Door& door) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// transform
		assembly::Transform::make(registry, entity, door.position);
		// render
		assembly::Render::make(registry, entity, door.sprite);
		// collision
		{
			const auto body_id = assembly::Collision::make_body(registry, entity, BodyDef, door.position);

			// 门
			const auto door_shape_id = assembly::Collision::make_shape(
				body_id,
				DoorShapeDef,
				blueprint::CollisionShape::OffsetBox
				{
						.center = {.x = door.door_offset.x, .y = door.door_offset.y},
						.size = door.door_size,
						.rotation = {.rotation = 0},
				}
			);
			registry.emplace<door::DoorShapeId>(entity, door_shape_id);

			// 感应区
			const auto sensor_shape_id = assembly::Collision::make_shape(
				body_id,
				SensorShapeDef,
				blueprint::CollisionShape::OffsetBox
				{
						.center = {.x = door.sensor_offset.x, .y = door.sensor_offset.y},
						.size = door.sensor_size,
						.rotation = {.rotation = 0},
				}
			);
			registry.emplace<door::SensorShapeId>(entity, sensor_shape_id);
		}
		// direction
		registry.emplace<door::Direction>(entity, door.direction);
		// state
		registry.emplace<door::State>(entity, door::State::CLOSED);
		// door::Room由factory::Room附加
		// door::TargetRoom由factory::Level附加
		// tags
		registry.emplace<tags::Door>(entity);

		// 新生成
		registry.emplace<state::EntityNew>(entity);

		return entity;
	}

	auto Door::destroy_all(entt::registry& registry) noexcept -> void
	{
		SPDLOG_INFO("正在销毁所有门...");

		const auto view = registry.view<tags::Door>();
		registry.destroy(view.begin(), view.end());

		SPDLOG_INFO("已销毁{}个门", view.size());
	}
}
