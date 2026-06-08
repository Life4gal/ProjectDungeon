// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <undertaker/scheduled_task.hpp>

#include <component/scheduled_task_context.hpp>

#include <entt/entt.hpp>

namespace pd::undertaker
{
	using namespace component;

	namespace
	{
		auto on_destroy(entt::registry& registry, const entt::entity entity) noexcept -> void
		{
			auto& [context] = registry.ctx().get<scheduled_task::Context>();

			const auto it = context.find(entity);
			if (it == context.end())
			{
				// 这种情况当且仅当factory::ScheduledTask::destroy_all调用过才可能发生?
				return;
			}

			const auto& tasks = it->second;

			// 销毁所有任务
			registry.destroy(tasks.begin(), tasks.end());
			// 删除映射
			context.erase(entity);
		}
	}

	auto ScheduledTask::watch(entt::registry& registry) noexcept -> void
	{
		registry.on_destroy<scheduled_task::Scheduled>().connect<&on_destroy>();
	}
}
