// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/scheduled_task.hpp>

#include <ranges>

#include <component/scheduled_task.hpp>
#include <component/transform.hpp>
#include <component/tags.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::update
{
	using namespace component;

	namespace
	{
		auto invoke_task(entt::registry& registry, const entt::entity owner, const scheduled_task::Task& task) noexcept -> void
		{
			// 避免我们不小心传递错了参数 :)
			PROMETHEUS_PLATFORM_ASSUME(not registry.all_of<tags::ScheduledTask>(owner));

			// // 实体会不会已经被销毁?
			// if (not registry.valid(owner))
			// {
			// 	return;
			// }

			if (task.ids.empty())
			{
				// 标记为可被销毁
				registry.emplace<state::EntityDead>(owner);

				return;
			}

			for (const auto [id, name]: std::views::zip(task.ids, task.names))
			{
				// 获取组件池
				auto* storage = registry.storage(id);
				if (storage == nullptr)
				{
					SPDLOG_WARN(
						"找不到指定组件池({}),无法执行任务!",
						name
					);

					return;
				}

				// 移除组件(从池中移除该实体)
				if (not storage->remove(owner))
				{
					SPDLOG_WARN(
						"实体(0x{:08x})不存在所需组件({}),任务执行失败!",
						entt::to_integral(owner),
						name
					);

					return;
				}

				//
			}
		}

		auto time(entt::registry& registry, const sf::Time delta) noexcept -> void
		{
			namespace stt = scheduled_task::time;

			for (const auto view = registry.view<const scheduled_task::Owner, const stt::Lifetime, stt::Elapsed, const scheduled_task::Task>();
			     const auto [entity, owner,lifetime, elapsed, task]: view.each())
			{
				elapsed.elapsed += delta;
				if (elapsed.elapsed >= lifetime.lifetime)
				{
					// 下一时间周期
					elapsed.elapsed -= lifetime.lifetime;
					// 执行任务
					invoke_task(registry, owner.owner, task);
				}
			}
		}

		auto distance(entt::registry& registry) noexcept -> void
		{
			namespace std = scheduled_task::distance;

			for (const auto view = registry.view<const scheduled_task::Owner, const std::DistanceSquared, std::AnchorPosition, const scheduled_task::Task>();
			     const auto [entity, owner, distance_squared, anchor_position, task]: view.each())
			{
				const auto* position = registry.try_get<transform::Position>(owner.owner);
				if (position == nullptr)
				{
					// 创建时必定存在该组件
					// 如果此时不存在该组件则说明transform::Position被移除
					SPDLOG_INFO(
						"因实体(0x{:08x})的transform::Position组件被移除,终止计划任务!",
						entt::to_integral(owner.owner)
					);

					registry.destroy(entity);
					continue;
				}

				const auto diff = position->position - anchor_position.position;
				const auto diff_squared = diff.lengthSquared();

				if (diff_squared >= distance_squared.value)
				{
					// 下一距离周期
					anchor_position.position = position->position;
					// 执行任务
					invoke_task(registry, owner.owner, task);
				}
			}
		}
	}

	auto scheduled_task(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		// 时间
		time(registry, delta);
		// 距离
		distance(registry);
	}
}
