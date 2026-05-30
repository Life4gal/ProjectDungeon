// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/level.hpp>

#include <blueprint/def_name.hpp>

#include <designer/room.hpp>

#include <component/level.hpp>
#include <component/door.hpp>

#include <factory/room.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::factory
{
	using namespace component;

	auto Level::create(entt::registry& registry, const blueprint::Level& level) noexcept -> void
	{
		// 有且仅有一个起始房间
		PROMETHEUS_PLATFORM_ASSUME(std::ranges::contains(level.rooms | std::views::values, blueprint::RoomType::START, &blueprint::Room::type) == 1);

		// 创建上下文
		auto& [entity_to_position] = registry.ctx().emplace<level::EntityToPosition>();
		auto& [position_to_entity] = registry.ctx().emplace<level::PositionToEntity>();
		auto& [position_to_neighbor] = registry.ctx().emplace<level::PositionToNeighbor>();

		entity_to_position.reserve(level.rooms.size());
		position_to_entity.reserve(level.rooms.size());
		position_to_neighbor.reserve(level.rooms.size());

		// 创建房间实体
		for (const auto& [position, room]: level.rooms)
		{
			const auto room_entity = Room::spawn(registry, room);

			entity_to_position.emplace(room_entity, position);
			position_to_entity.emplace(position, room_entity);
			position_to_neighbor.emplace(position, std::array<entt::entity, 4>{{entt::null, entt::null, entt::null, entt::null}});
		}
		// 创建位置与房间邻居的映射
		{
			const auto find_neighbor = [&position_to_entity](const blueprint::LayoutPosition position, const blueprint::Direction neighbor) noexcept -> entt::entity
			{
				const auto [x, y] = designer::RoomNavigation::direction_of(neighbor);
				const blueprint::LayoutPosition next_position{.x = position.x + x, .y = position.y + y};

				const auto it = position_to_entity.find(next_position);
				if (it == position_to_entity.end())
				{
					return entt::null;
				}

				return it->second;
			};

			for (const auto& position: level.rooms | std::views::keys)
			{
				auto& neighbors = position_to_neighbor[position];

				// NORTH
				neighbors[std::to_underlying(blueprint::Direction::NORTH)] = find_neighbor(position, blueprint::Direction::NORTH);
				// SOUTH
				neighbors[std::to_underlying(blueprint::Direction::SOUTH)] = find_neighbor(position, blueprint::Direction::SOUTH);
				// WEST
				neighbors[std::to_underlying(blueprint::Direction::WEST)] = find_neighbor(position, blueprint::Direction::WEST);
				// EAST
				neighbors[std::to_underlying(blueprint::Direction::EAST)] = find_neighbor(position, blueprint::Direction::EAST);
			}
		}

		// 给所有门实体设置目标房间
		{
			const auto do_set = [&](
				door::TargetRoom& target_room,
				const entt::entity room,
				const blueprint::LayoutPosition room_position,
				const entt::entity door,
				const door::Direction door_direction,
				const entt::entity neighbor_room
			) noexcept -> void
			{
				const auto [neighbor_room_x, neighbor_room_y] = entity_to_position.at(neighbor_room);

				SPDLOG_INFO(
					"设置房间(0x{:08X})[{}.{}]位于({})的门(0x{:08X})的目标房间为(0x{:08X})[{}.{}]",
					entt::to_integral(room),
					room_position.x,
					room_position.y,
					prometheus::meta::name_of(door_direction),
					entt::to_integral(door),
					entt::to_integral(neighbor_room),
					neighbor_room_x,
					neighbor_room_y
				);

				target_room.room = neighbor_room;
			};

			for (const auto view = registry.view<tags::Door, door::Direction, door::TargetRoom>();
			     const auto [entity, direction, target_room]: view.each())
			{
				// target_room一开始设置为door所属房间
				const auto room_entity = target_room.room;

				const auto it_room_position = entity_to_position.find(room_entity);
				if (it_room_position == entity_to_position.end())
				{
					SPDLOG_WARN("门感应器(0x{:08X})所属房间(0x{:08X})实体不存在!", entt::to_integral(entity), entt::to_integral(room_entity));
					continue;
				}
				const auto room_position = it_room_position->second;

				const auto it_room_neighbors = position_to_neighbor.find(room_position);
				if (it_room_neighbors == position_to_neighbor.end())
				{
					SPDLOG_WARN("房间(0x{:08X})[{}.{}]不存在邻居房间!", entt::to_integral(room_entity), room_position.x, room_position.y);
					continue;
				}
				const auto& room_neighbors = it_room_neighbors->second;
				const auto neighbor_entity = room_neighbors[std::to_underlying(direction)];

				do_set(target_room, room_entity, room_position, entity, direction, neighbor_entity);
			}
		}
	}

	auto Level::destroy(entt::registry& registry) noexcept -> void
	{
		Room::destroy_all(registry);

		registry.ctx().erase<level::EntityToPosition>();
		registry.ctx().erase<level::PositionToEntity>();
		registry.ctx().erase<level::PositionToNeighbor>();
	}
}
