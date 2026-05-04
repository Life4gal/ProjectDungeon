// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/level.hpp>

#include <designer/room.hpp>

#include <component/door.hpp>

#include <factory/room.hpp>

#include <prometheus/meta/enumeration.hpp>
#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

template<>
struct prometheus::meta::user_defined::enum_name_policy<pd::component::door::Direction>
{
	constexpr static auto value = EnumNamePolicy::VALUE_ONLY;
};

namespace pd::factory
{
	auto Level::create(entt::registry& registry, const blueprint::Level& level) noexcept -> void
	{
		using position_type = blueprint::RoomPosition;

		// 房间实体与位置的映射
		std::unordered_map<entt::entity, position_type> entity_to_positions{};
		// 位置与房间实体的映射
		std::unordered_map<position_type, entt::entity> position_to_entities{};
		// 位置与房间邻居的映射
		std::unordered_map<position_type, std::array<entt::entity, 4>> position_to_neighbors{};

		// 创建房间实体
		for (const auto& [position, room]: level.rooms)
		{
			const auto room_entity = Room::spawn(registry, room);

			entity_to_positions.emplace(room_entity, position);
			position_to_entities.emplace(position, room_entity);
		}
		// 创建位置与房间邻居的映射
		{
			const auto find_neighbor = [&position_to_entities](const position_type position, const blueprint::RoomConnection neighbor) noexcept -> entt::entity
			{
				const auto [x, y] = designer::RoomNavigation::direction_of(neighbor);
				const position_type next_position{.x = position.x + x, .y = position.y + y};

				const auto it = position_to_entities.find(next_position);
				if (it == position_to_entities.end())
				{
					return entt::null;
				}

				return it->second;
			};

			for (const auto& position: level.rooms | std::views::keys)
			{
				auto& neighbors = position_to_neighbors[position];

				// NORTH
				neighbors[0] = find_neighbor(position, blueprint::RoomConnection::NORTH);
				// SOUTH
				neighbors[1] = find_neighbor(position, blueprint::RoomConnection::SOUTH);
				// WEST
				neighbors[2] = find_neighbor(position, blueprint::RoomConnection::WEST);
				// EAST
				neighbors[3] = find_neighbor(position, blueprint::RoomConnection::EAST);
			}
		}

		// 给所有门实体设置目标房间
		{
			using namespace component;

			const auto do_set = [&](
				door::TargetRoom& target_room,
				const entt::entity room,
				const position_type room_position,
				const entt::entity door,
				const door::Direction door_direction,
				const entt::entity neighbor_room
			) noexcept -> void
			{
				const auto neighbor_room_position = entity_to_positions.at(neighbor_room);

				SPDLOG_INFO(
					"设置房间(0x{:08X})[{}.{}]位于({})的门(0x{:08X})的目标房间为(0x{:08X})[{}.{}]",
					entt::to_integral(room),
					room_position.x,
					room_position.y,
					prometheus::meta::name_of(door_direction),
					entt::to_integral(door),
					entt::to_integral(neighbor_room),
					neighbor_room_position.x,
					neighbor_room_position.y
				);

				target_room.target_room = neighbor_room;
			};

			for (const auto view = registry.view<door::Door, door::Direction, door::TargetRoom>();
			     const auto [entity, direction, target_room]: view.each())
			{
				// target_room一开始设置为door所属房间
				const auto room_entity = target_room.target_room;

				const auto it_room_position = entity_to_positions.find(room_entity);
				if (it_room_position == entity_to_positions.end())
				{
					SPDLOG_WARN("门(0x{:08X})所属房间(0x{:08X})实体不存在!", entt::to_integral(entity), entt::to_integral(room_entity));
					continue;
				}
				const auto room_position = it_room_position->second;

				const auto it_room_neighbors = position_to_neighbors.find(room_position);
				if (it_room_neighbors == position_to_neighbors.end())
				{
					SPDLOG_WARN("房间(0x{:08X})[{}.{}]不存在邻居房间!", entt::to_integral(room_entity), room_position.x, room_position.y);
					continue;
				}
				const auto& room_neighbors = it_room_neighbors->second;

				const auto neighbor_entity = [&] noexcept -> entt::entity
				{
					if (direction == door::Direction::NORTH)
					{
						return room_neighbors[0];
					}

					if (direction == door::Direction::SOUTH)
					{
						return room_neighbors[1];
					}

					if (direction == door::Direction::WEST)
					{
						return room_neighbors[2];
					}

					if (direction == door::Direction::EAST)
					{
						return room_neighbors[3];
					}

					PROMETHEUS_PLATFORM_UNREACHABLE();
				}();

				do_set(target_room, room_entity, room_position, entity, direction, neighbor_entity);
			}
		}
	}

	auto Level::destroy(entt::registry& registry) noexcept -> void
	{
		Room::destroy_all(registry);
	}
}
