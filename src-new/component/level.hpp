// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/level.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::component::level
{
	// 与camera::Dirty类似
	// 当房间切换时添加该标记
	//
	// [CTX]
	class RoomChanged {};

	// 房间实体与位置的映射
	// 房间实体本身也保有其所在位置的组件,所以该组件应该用不到(只在factory::Level::create中用到)
	//
	// [CTX]
	class EntityToPosition final
	{
	public:
		std::unordered_map<entt::entity, blueprint::LayoutPosition> entity_to_position;
	};

	// 位置与房间实体的映射
	// 可以基于布局位置直接查找房间实体
	// 一般用于直接控制某个确定位置的房间实体
	//
	// [CTX]
	class PositionToEntity final
	{
	public:
		std::unordered_map<blueprint::LayoutPosition, entt::entity> position_to_entity;
	};

	// 位置与房间邻居的映射
	// array中的顺序为blueprint::Direction的顺序(NORTH, SOUTH, WEST, EAST)
	// array[index]的实体可能为null,此时表示该方向没有邻居
	// 每个门实体都会保有其目标房间的组件,所以该组件应该用不到(只在factory::Level::create中用到)
	//
	// [CTX]
	class PositionToNeighbor final
	{
	public:
		std::unordered_map<blueprint::LayoutPosition, std::array<entt::entity, 4>> position_to_neighbor;
	};
}
