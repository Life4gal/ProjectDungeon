// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/room.hpp>

#include <algorithm>
#include <ranges>

#include <components/tags.hpp>
#include <components/transform.hpp>
#include <components/physics_world.hpp>

#include <systems/physics_world.hpp>

#include <prometheus/platform/os.hpp>

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace pd::systems
{
	namespace
	{
		auto current_room(entt::registry& registry) -> components::room::Room&
		{
			using namespace components;

			auto& [room] = registry.ctx().get<room::CurrentRoom>();

			return room.get();
		}
	}

	auto Room::create(entt::registry& registry) noexcept -> void
	{
		//
	}

	auto Room::destroy(entt::registry& registry) noexcept -> void
	{
		//
	}

	auto Room::open_doors(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		for (const auto view = registry.view<room::Door, physics_world::BodyId>(
			     entt::exclude<
				     // 排除非当前房间门
				     tags::disabled,
				     // 排除锁上的门
				     room::LockedDoor
			     >
		     );
		     const auto [entity, door, body_id]: view.each())
		{
			if (door.state == room::DoorState::CLOSING or door.state == room::DoorState::CLOSED)
			{
				door.state = room::DoorState::OPENING;
				door.timer_ms = 0;
			}

			// 移除门的物理碰撞体
			PhysicsWorld::deattach(registry, entity);
		}
	}

	auto Room::close_doors(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		for (const auto view = registry.view<room::Door, transform::Position, transform::Scale, transform::Rotation>(
			     entt::exclude<
				     // 排除非当前房间门
				     tags::disabled
			     >
		     );
		     const auto [entity, door, position, scale, rotation]: view.each())
		{
			// 理论上其不应该有物理碰撞体,如果有则说明之前的门没有被正确移除
			PROMETHEUS_PLATFORM_ASSUME(registry.all_of<physics_world::BodyId>(entity));

			if (door.state == room::DoorState::OPENING or door.state == room::DoorState::OPENED)
			{
				door.state = room::DoorState::CLOSING;
				door.timer_ms = 0;
			}

			// 创建门的物理碰撞体
			const auto world_id = PhysicsWorld::id(registry);

			auto body_def = b2DefaultBodyDef();
			body_def.type = b2_staticBody;
			body_def.position = PhysicsWorld::physics_position_of(position.position);
			body_def.rotation = PhysicsWorld::physics_rotation_of(rotation.rotation);
			body_def.userData = PhysicsWorld::to_user_data(entity);

			PhysicsWorld::attach(registry, entity, world_id, body_def);

			const auto [half_width, half_height] = PhysicsWorld::physics_size_of({game::room_tile_width * 0.5f * scale.scale.x, game::room_tile_height * 0.5f * scale.scale.y});
			const auto box = b2MakeBox(half_width, half_height);

			auto shape_def = b2DefaultShapeDef();
			shape_def.density = 0;
			shape_def.material.friction = 0;

			PhysicsWorld::attach_shape(registry, entity, shape_def, box);
		}
	}

	auto Room::tile(entt::registry& registry, const size_type x, const size_type y) noexcept -> const components::room::Tile*
	{
		using namespace components;

		if (x >= game::room_horizontal_grid or y >= game::room_vertical_grid)
		{
			return nullptr;
		}

		const auto& room = current_room(registry);

		return &room.grid[x,y];
	}

	auto Room::is_walkable(entt::registry& registry, const size_type x, const size_type y) noexcept -> bool
	{
		using namespace components;

		const auto* tile = Room::tile(registry, x, y);
		if (tile == nullptr)
		{
			return false;
		}

		return tile->collision_logical == room::CollisionLogical::FLOOR;
	}

	auto Room::is_flyable(entt::registry& registry, const size_type x, const size_type y) noexcept -> bool
	{
		using namespace components;

		const auto* tile = Room::tile(registry, x, y);
		if (tile == nullptr)
		{
			return false;
		}

		return tile->collision_logical != room::CollisionLogical::WALL;
	}

	auto Room::is_passable(entt::registry& registry, const size_type x, const size_type y) noexcept -> bool
	{
		using namespace components;

		const auto* tile = Room::tile(registry, x, y);
		if (tile == nullptr)
		{
			return false;
		}

		return tile->collision_logical != room::CollisionLogical::WALL and tile->collision_logical != room::CollisionLogical::OBSTACLE;
	}
}
