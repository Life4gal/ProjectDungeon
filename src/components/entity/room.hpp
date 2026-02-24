// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>
#include <vector>

#include <entt/entity/fwd.hpp>

namespace pd::components::room
{
	class Id final
	{
	public:
		// 房间ID
		// todo: 需要吗?
		std::string room_id;
	};

	class TileEntities final
	{
	public:
		// 房间内的所有瓦片实体
		std::vector<entt::entity> entities;
	};

	class DoorEntities final
	{
	public:
		// 房间内的门实体列表
		std::vector<entt::entity> doors;
	};

	class EnemyEntities final
	{
	public:
		// 房间内的敌人实体列表
		std::vector<entt::entity> enemies;
	};
}
