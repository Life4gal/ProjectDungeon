// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::component::schedule_destroy
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
	// 销毁模式
	// ===================================

	// 立即销毁: registry.destroy(entity)
	// 延迟销毁: registry.emplace<state::EntityDead>(entity)
	enum class Mode : std::uint8_t
	{
		INSTANCE,
		DELAY,
	};
}
