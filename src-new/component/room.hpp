// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <component/tags.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::component::room
{
	// tags

	// 当前房间的门实体
	class Doors final
	{
	public:
		std::vector<entt::entity> doors;
	};

	// 当前房间的敌人实体
	// 在生成房间时填充列表
	// 在敌人死亡时替换为null
	// 在所有元素全部为null时打开门
	// 在离开房间时删除该组件
	class Enemies final
	{
	public:
		std::vector<entt::entity> enemies;
	};
}
