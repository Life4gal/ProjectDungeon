// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <component/collision.hpp>
#include <component/tags.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::component::bounding
{
	// collision --> 房间边界碰撞体
	// tags --> 房间边界标签

	// 所有边界形状ID
	class ShapeIds final
	{
	public:
		std::vector<b2ShapeId> shape_ids;
	};

	// ============================================

	// 房间边界所属房间实体
	// 该组件不由factory::Bounding附加,而是由factory::Room附加
	class Room final
	{
	public:
		entt::entity room;
	};
}
