// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <designer/door.hpp>

#include <designer/room.hpp>

#include <prometheus/platform/os.hpp>

namespace pd::designer
{
	namespace
	{
		// "door"占整个碰撞体比例
		constexpr float DoorBodyRatio = 0.6667f;
		// "sensor"占整个碰撞体比例
		constexpr float DoorSensorRatio = 0.2333f;
		// "blocker"占整个碰撞体比例
		constexpr float DoorBlockerRatio = 1 - DoorBodyRatio - DoorSensorRatio;
	}

	auto Door::standard(const size_type tile_x, const size_type tile_y, const blueprint::DoorDirection direction) noexcept -> blueprint::Door
	{
		struct desc_type
		{
			float width;
			float height;
			float offset_x;
			float offset_y;
		};

		desc_type door_desc{};
		desc_type sensor_desc{};
		desc_type blocker_desc{};

		if (direction == blueprint::DoorDirection::NORTH or direction == blueprint::DoorDirection::SOUTH)
		{
			constexpr auto half_tile_height = Room::tile_height * 0.5f;

			door_desc.width = Room::tile_width;
			door_desc.height = Room::tile_height * DoorBodyRatio;
			door_desc.offset_x = 0;

			sensor_desc.width = Room::tile_width;
			sensor_desc.height = Room::tile_height * DoorSensorRatio;
			sensor_desc.offset_x = 0;

			blocker_desc.width = Room::tile_width;
			blocker_desc.height = Room::tile_height * DoorBlockerRatio;
			blocker_desc.offset_x = 0;

			if (direction == blueprint::DoorDirection::NORTH)
			{
				door_desc.offset_y = half_tile_height - door_desc.height * 0.5f;
				sensor_desc.offset_y = half_tile_height - door_desc.height - sensor_desc.height * 0.5f;
				blocker_desc.offset_y = -half_tile_height + blocker_desc.height * 0.5f;
			}
			else
			{
				door_desc.offset_y = -half_tile_height + door_desc.height * 0.5f;
				sensor_desc.offset_y = -half_tile_height + door_desc.height + sensor_desc.height * 0.5f;
				blocker_desc.offset_y = half_tile_height - blocker_desc.height * 0.5f;
			}
		}
		else if (direction == blueprint::DoorDirection::WEST or direction == blueprint::DoorDirection::EAST)
		{
			constexpr auto half_tile_width = Room::tile_width * 0.5f;

			door_desc.width = Room::tile_width * DoorBodyRatio;
			door_desc.height = Room::tile_height;
			door_desc.offset_y = 0;

			sensor_desc.width = Room::tile_width * DoorSensorRatio;
			sensor_desc.height = Room::tile_height;
			sensor_desc.offset_y = 0;

			blocker_desc.width = Room::tile_width * DoorBlockerRatio;
			sensor_desc.height = Room::tile_height;
			sensor_desc.offset_y = 0;

			if (direction == blueprint::DoorDirection::WEST)
			{
				door_desc.offset_x = half_tile_width - door_desc.width * 0.5f;
				sensor_desc.offset_x = half_tile_width - door_desc.width - sensor_desc.width * 0.5f;
				blocker_desc.offset_x = -half_tile_width + blocker_desc.width * 0.5f;
			}
			else
			{
				door_desc.offset_x = -half_tile_width + door_desc.width * 0.5f;
				sensor_desc.offset_x = -half_tile_width + door_desc.width + sensor_desc.width * 0.5f;
				blocker_desc.offset_x = half_tile_width - blocker_desc.width * 0.5f;
			}
		}
		else
		{
			PROMETHEUS_PLATFORM_UNREACHABLE();
		}

		const blueprint::Position position
		{
				.x = static_cast<float>(Room::tile_origin_x + tile_x * Room::tile_width),
				.y = static_cast<float>(Room::tile_origin_y + tile_y * Room::tile_height),
		};
		blueprint::Sprite sprite
		{
				.texture = "./assets/tileset/door.png",
				.position = {.x = 0, .y = 0},
				.size = {.width = Room::tile_width, .height = Room::tile_height},
				.origin = {.x = Room::tile_origin_x, .y = Room::tile_origin_y},
				.scale = {.x = 1, .y = 1},
		};
		constexpr blueprint::PhysicsBody physics_body{.type = blueprint::PhysicsBodyType::STATIC, .fixed_rotation = true, .is_bullet = false};
		const blueprint::PhysicsShapeOffsetBox physics_shape_door
		{
				.def =
				{
						.material = {.friction = 0.6f, .restitution = 0},
						.density = 0,
						.category = blueprint::PhysicsShapeType::DOOR,
						.category_mask = blueprint::PhysicsShapeCollisionMask::door_close,
						.is_sensor = false,
						.enable_sensor_events = false,
						.enable_contact_events = true,
				},
				.size = {.width = door_desc.width, .height = door_desc.height},
				.offset = {.x = door_desc.offset_x, .y = door_desc.offset_y},
				.rotation = 0,
		};
		const blueprint::PhysicsShapeOffsetBox physics_shape_sensor
		{
				.def =
				{
						.material = {.friction = 0, .restitution = 0},
						.density = 0,
						.category = blueprint::PhysicsShapeType::DOOR,
						.category_mask = blueprint::PhysicsShapeCollisionMask::door_sensor,
						.is_sensor = true,
						.enable_sensor_events = true,
						.enable_contact_events = false,
				},
				.size = {.width = sensor_desc.width, .height = sensor_desc.height},
				.offset = {.x = sensor_desc.offset_x, .y = sensor_desc.offset_y},
				.rotation = 0,
		};
		const blueprint::PhysicsShapeOffsetBox physics_shape_blocker
		{
				.def =
				{
						.material = {.friction = 0, .restitution = 0},
						.density = 0,
						.category = blueprint::PhysicsShapeType::DOOR,
						.category_mask = blueprint::PhysicsShapeCollisionMask::door_blocker,
						.is_sensor = false,
						.enable_sensor_events = false,
						.enable_contact_events = false,
				},
				.size = {.width = blocker_desc.width, .height = blocker_desc.height},
				.offset = {.x = blocker_desc.offset_x, .y = blocker_desc.offset_y},
				.rotation = 0,
		};

		return
		{
				.position = position,
				.sprite = std::move(sprite),
				.physics_body = physics_body,
				.physics_shape_door = physics_shape_door,
				.physics_shape_sensor = physics_shape_sensor,
				.physics_shape_blocker = physics_shape_blocker,
				.direction = direction
		};
	}
}
