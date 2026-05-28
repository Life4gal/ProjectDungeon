// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/door.hpp>

#include <component/door.hpp>

#include <factory/detail/transform.hpp>
#include <factory/detail/render.hpp>
#include <factory/detail/collision.hpp>

#include <entt/entt.hpp>

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

	auto Door::spawn(entt::registry& registry, const blueprint::Door& door, const blueprint::Direction direction) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// transform
		detail::attach(registry, entity, door.position);
		// render
		detail::attach(registry, entity, door.sprite, blueprint::RenderLayer::DOOR);
		// collision
		{
			const auto body_id = detail::create_attach(registry, entity, BodyDef, door.position);

			// 门
			const auto door_shape_id = detail::create(body_id, DoorShapeDef, blueprint::CollisionShape::box{.size = door.size});
			registry.emplace<door::DoorShapeId>(entity, door_shape_id);

			// 感应区
			const auto sensor_shape_id = detail::create(
				body_id,
				SensorShapeDef,
				blueprint::CollisionShape::offset_box
				{
						.center = {.x = door.sensor_position.x, .y = door.sensor_position.y},
						.size = door.sensor_size,
						.rotation = {.rotation = 0}
				}
			);
			registry.emplace<door::SensorShapeId>(entity, sensor_shape_id);
		}
		// direction
		registry.emplace<door::Direction>(entity, direction);
		// door::Room由factory::Room附加
		// door::TargetRoom由factory::Level附加
		// tags
		registry.emplace<tags::Door>(entity);

		return entity;
	}

	auto Door::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<tags::Door>();
		registry.destroy(view.begin(), view.end());
	}
}
