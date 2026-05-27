// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <array>
#include <vector>

#include <component/door_sensor.hpp>

#include <SFML/System/Vector2.hpp>

namespace pd::component::room
{
	// collision --> 房间碰撞体(用于房间边界)
	// tags --> 房间标签

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
	// 门感应区
	// ============================================

	class DoorSensors final
	{
	public:
		// 如有有 -> valid entity
		// 如果没有 -> null
		std::array<entt::entity, 4> sensors;
	};

	// ============================================
	// 房间边界
	// ============================================

	class Bounding final
	{
	public:
		std::vector<b2ShapeId> shapes;
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
	// 由factory::Room创建敌人时设置
	// 在敌人死亡时替换为null
	// 在所有元素全部为null时打开门
	// 在离开房间时删除该组件(也就是说不存在该组件的房间视为*已清理*)
	class Enemies final
	{
	public:
		std::vector<entt::entity> enemies;
	};
}
