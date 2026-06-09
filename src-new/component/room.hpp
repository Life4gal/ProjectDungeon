// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <array>
#include <vector>

#include <component/door.hpp>
#include <component/bounding.hpp>
#include <component/tile.hpp>
#include <component/enemy.hpp>

namespace pd::component::room
{
	// tags --> 房间标签

	// ============================================
	// 类型
	// ============================================

	using blueprint::RoomType;

	// ============================================
	// 位置 + 大小
	// ============================================

	// 房间位置(布局位置)
	class LayoutPosition final
	{
	public:
		sf::Vector2u position;
	};

	// 房间位置(绝对位置)
	// 理论上每个房间的绝对位置是确定的(取决于蓝图),且其屏幕位置一定为(0,0)
	class Position final
	{
	public:
		sf::Vector2f position;
	};

	// 房间大小
	// 理论上每个房间的大小相同?
	class Size final
	{
	public:
		sf::Vector2f size;
	};

	// ============================================
	// 边界(墙壁)
	// ============================================

	class Bounding final
	{
	public:
		entt::entity bounding;
	};

	// ============================================
	// 门
	// ============================================

	class Doors final
	{
	public:
		// 如有有 -> valid entity
		// 如果没有 -> null
		std::array<entt::entity, 4> doors;
	};

	// ============================================
	// 瓦片
	// ============================================

	class Tiles final
	{
	public:
		std::vector<entt::entity> tiles;
	};

	// ============================================
	// 敌人
	// ============================================

	// 当前房间的敌人实体
	// 所有元素均为无效实体时视为房间已清理
	// 不存在该组件视为房间已清理
	class Enemies final
	{
	public:
		std::vector<entt::entity> enemies;
	};

	// ============================================
	// NPC
	// ============================================

	// // 当前房间的NPC实体
	// class Npc final
	// {
	// public:
	// 	std::vector<entt::entity> npc;
	// };
}
