// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::events
{
	// ===============
	// 门
	// ===============

	// 与门实体接触时触发
	class ClosedDoorContacted final
	{
	public:
		entt::entity door;
		entt::entity player;
	};

	// 与门感应器接触时触发
	class OpenedDoorContacted final
	{
	public:
		entt::entity door;
		entt::entity player;
	};

	// ===============
	// 宝箱
	// ===============

	// 仅与未开启宝箱接触时触发
	class ChestContacted final
	{
	public:
		entt::entity chest;
		entt::entity player;

		// 宝箱在CacheChests中的索引
		// 有这个索引,我们不再需要获取宝箱实体的位置信息,也不用限制一个房间只有一个宝箱
		std::uint32_t chest_index;
	};

	// ===============
	// 可破坏物
	// ===============

	// 与可破坏物接触时触发
	// 在事件级别就区分与其解除的实体类型,还是由订阅者自行判断?
	class DestroyableObjectContacted final
	{
	public:
		entt::entity destroyable_object;
		entt::entity contactor;
	};

	// ===============
	// 物品
	// ===============

	// 与物品接触时触发
	class ItemContacted final
	{
	public:
		entt::entity item;
		entt::entity player;
	};

	// ===============
	// 尸体
	// ===============

	// 生成尸体时触发
	class CorpseCreated final
	{
	public:
		entt::entity corpse;
	};

	// 与尸体接触时触发
	class CorpseContacted final
	{
	public:
		entt::entity corpse;
		entt::entity contactor;
	};
}
