// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <helper/level.hpp>

#include <designer/room.hpp>

#include <component/level.hpp>
#include <component/room.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>

namespace pd::helper
{
	using namespace component;

	auto Level::register_room(entt::registry& registry, const blueprint::LayoutPosition position, const entt::entity room) noexcept -> void
	{
		auto& [entity_to_position] = registry.ctx().get<level::EntityToPosition>();
		auto& [position_to_entity] = registry.ctx().get<level::PositionToEntity>();
		auto& [position_to_neighbor] = registry.ctx().get<level::PositionToNeighbor>();

		// 注册实体与布局映射
		entity_to_position[room] = position;
		position_to_entity[position] = room;

		// 注册位置与邻居映射
		auto& neighbors = position_to_neighbor[position];
		neighbors.fill(entt::null);

		// 与邻居相互连接
		{
			const auto connect_neighbor = [&](const blueprint::Direction direction) noexcept -> void
			{
				const auto [x,y] = designer::RoomNavigation::direction_of(direction);
				const blueprint::LayoutPosition next_position{.x = position.x + x, .y = position.y + y};
				const auto it = position_to_entity.find(next_position);

				if (it == position_to_entity.end())
				{
					// 有两种情况:
					//  1.该方向没有邻居
					//  2.该邻居房间还未创建
					// 两种情况都无需处理
					return;;
				}

				const auto neighbor = it->second;

				// 连接房间分4步:
				// 1: 将目标房间注册为当前房间邻居
				// 2: 将当前房间注册为目标房间邻居
				// 3. 将当前房间门目标指定为目标房间
				// 4. 将目标房间门目标指定为当前房间

				// 1: 将目标房间注册为当前房间邻居
				neighbors[std::to_underlying(direction)] = neighbor;

				// 2: 将当前房间注册为目标房间邻居
				auto& neighbor_neighbors = position_to_neighbor[next_position];
				// 注意是反方向
				neighbor_neighbors[std::to_underlying(-direction)] = room;

				// 3. 将当前房间门目标指定为目标房间
				auto& [room_doors] = registry.get<room::Doors>(room);
				const auto room_door = room_doors[std::to_underlying(direction)];
				PROMETHEUS_PLATFORM_ASSUME(registry.valid(room_door));
				registry.emplace_or_replace<door::TargetRoom>(room_door, neighbor);

				// 4. 将目标房间门目标指定为当前房间
				auto& [neighbor_doors] = registry.get<room::Doors>(neighbor);
				// 注意是反方向
				const auto neighbor_door = neighbor_doors[std::to_underlying(-direction)];
				PROMETHEUS_PLATFORM_ASSUME(registry.valid(neighbor_door));
				registry.emplace_or_replace<door::TargetRoom>(neighbor_door, room);
			};

			connect_neighbor(blueprint::Direction::NORTH);
			connect_neighbor(blueprint::Direction::SOUTH);
			connect_neighbor(blueprint::Direction::WEST);
			connect_neighbor(blueprint::Direction::EAST);
		}
	}
}
