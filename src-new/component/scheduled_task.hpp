// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <component/tags.hpp>

#include <entt/core/fwd.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

namespace pd::component::scheduled_task
{
	// tags --> 计划任务标签

	// 计划任务是独立实体:
	// 优点:
	//  一个实体可以同时有多个计划任务(显然如此,因为任务与实体本身不再存在强关联)
	// 缺点:
	//  没法利用registry.view获得需要的实体,只能registry.get<T>(owner),性能稍差
	//  实体销毁没法立刻停止所有所属的计划任务,也就是说只能让任务本身去确定所属实体是否健在
	//
	// FIXME(OPT): 
	//  目前有两个想法:
	//   1.让实体本身维护一个任务列表,实体销毁时顺便销毁所有任务实体
	//   2.直接在上下文中维护一个实体与任务列表的映射,实体销毁时从映射中移除列表
	// 目前使用方案2
	//
	// TODO: 如何让任务停下来?

	// =========================================
	// 所属实体
	// =========================================

	class Owner
	{
	public:
		entt::entity owner;
	};

	// =========================================
	// 计划
	// =========================================

	// =================
	// 时间
	// =================

	namespace time
	{
		// 周期时间
		class Lifetime final
		{
		public:
			sf::Time lifetime;
		};

		// 当前周期已历时
		class Elapsed final
		{
		public:
			sf::Time elapsed;
		};
	}

	// =================
	// 距离
	// =================

	namespace distance
	{
		// 距离的平方
		class DistanceSquared final
		{
		public:
			float value;
		};

		// (当前周期)锚定位置
		class AnchorPosition final
		{
		public:
			sf::Vector2f position;
		};
	}

	// =================
	//
	// =================

	// =========================================
	// 任务
	// =========================================

	class Task final
	{
	public:
		using id_type = entt::id_type;

		// 如果列表为空则直接销毁实体
		std::vector<id_type> ids;

		// 便于调试
		std::vector<std::string_view> names;
	};
}
