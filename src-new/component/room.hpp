// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <component/tags.hpp>

#include <entt/entity/fwd.hpp>

#include <SFML/System/Vector2.hpp>

namespace pd::component::room
{
	// tags --> 房间标签

	// 房间位置(绝对位置)
	// 这里不使用position组件
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

	// 当前房间的门实体
	// 由factory::Room创建门时设置
	class Doors final
	{
	public:
		std::vector<entt::entity> doors;
	};

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
