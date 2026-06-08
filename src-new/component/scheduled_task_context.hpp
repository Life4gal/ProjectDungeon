// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>
#include <unordered_map>

#include <entt/entity/fwd.hpp>

namespace pd::component::scheduled_task
{
	// 这个标签附加给那些创建了计划任务的实体
	class Scheduled final {};

	// 实体与任务列表的映射
	//
	// [CTX]
	class Context final
	{
	public:
		using owner_type = entt::entity;
		using tasks_type = std::vector<entt::entity>;

		std::unordered_map<owner_type, tasks_type> context;
	};
}
