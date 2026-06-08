// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <variant>
#include <vector>

namespace pd::blueprint
{
	// 计划任务
	class ScheduledTask final
	{
	public:
		// =========================================
		// 计划
		// =========================================

		// 时间周期
		class TimeCycle final
		{
		public:
			// 周期时间(毫秒)
			int time_ms;
		};

		// 距离周期
		class DistanceCycle final
		{
		public:
			// 周期距离
			float distance;
		};

		using scheduled_type = std::variant<
			TimeCycle,
			DistanceCycle
		>;

		// =========================================
		// 任务
		// =========================================

		// 销毁实体
		class DestroyEntity final {};

		// 移除组件
		class RemoveComponent final
		{
		public:
			// using id_type = entt::id_type;
			using id_type = std::uint32_t;
			using ids_type = std::vector<id_type>;

			// id = entt::type_hash<T>::value()
			// 如果列表为空则直接销毁实体
			ids_type ids;
		};

		using task_type = std::variant<
			DestroyEntity,
			RemoveComponent
		>;

		scheduled_type scheduled;
		task_type task;
	};
}
