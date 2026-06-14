// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::component::schedule_component
{
	// =============
	// 调度器单独实体
	// =============

	// 调度器
	class Scheduler final
	{
	public:
		entt::entity scheduler;
	};

	// =============
	// 调度器附加在所有者上
	// =============

	//

	// ===================================
	// 要移除的组件
	// ===================================

	class Components final
	{
	public:
		using id_type = entt::id_type;

		// 组件id
		std::vector<id_type> ids;
		// 组件名(便于调试)
		std::vector<std::string_view> names;
	};
}
