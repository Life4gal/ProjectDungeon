// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/scheduled_task.hpp>

#include <component/scheduled_task.hpp>
#include <component/scheduled_task_context.hpp>
#include <component/transform.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::factory
{
	using namespace component;

	namespace
	{
		auto register_task(entt::registry& registry, const entt::entity entity, const entt::entity owner) noexcept -> void
		{
			auto& [context] = registry.ctx().get<scheduled_task::Context>();

			// 标记(注意其有可能已经被标记)
			registry.emplace_or_replace<scheduled_task::Scheduled>(owner);
			// 映射
			context[owner].emplace_back(entity);
		}
	}

	auto ScheduledTask::spawn(
		entt::registry& registry,
		const entt::entity owner,
		const blueprint::ScheduledTask::TimeCycle& time_cycle,
		blueprint::ScheduledTask::RemoveComponent::ids_type ids,
		std::vector<std::string_view> names
	) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// owner
		registry.emplace<scheduled_task::Owner>(entity, owner);
		// time
		registry.emplace<scheduled_task::time::Lifetime>(entity, sf::milliseconds(time_cycle.time_ms));
		registry.emplace<scheduled_task::time::Elapsed>(entity, sf::Time::Zero);
		// task
		registry.emplace<scheduled_task::Task>(entity, scheduled_task::Task{.ids = std::move(ids), .names = std::move(names)});
		register_task(registry, entity, owner);
		// tags
		registry.emplace<tags::ScheduledTask>(entity);

		return entity;
	}

	auto ScheduledTask::spawn(
		entt::registry& registry,
		const entt::entity owner,
		const blueprint::ScheduledTask::DistanceCycle& distance_cycle,
		blueprint::ScheduledTask::RemoveComponent::ids_type ids,
		std::vector<std::string_view> names
	) noexcept -> entt::entity
	{
		const auto* position = registry.try_get<transform::Position>(owner);
		if (position == nullptr)
		{
			SPDLOG_WARN(
				"尝试为实体(0x{:08x})配置距离任务,但是实体不含transform::Position组件!任务跳过!",
				entt::to_integral(owner)
			);

			return entt::null;
		}

		const auto entity = registry.create();

		// owner
		registry.emplace<scheduled_task::Owner>(entity, owner);
		// distance
		registry.emplace<scheduled_task::distance::DistanceSquared>(entity, distance_cycle.distance * distance_cycle.distance);
		registry.emplace<scheduled_task::distance::AnchorPosition>(entity, position->position);
		// task
		registry.emplace<scheduled_task::Task>(entity, scheduled_task::Task{.ids = std::move(ids), .names = std::move(names)});
		register_task(registry, entity, owner);
		// tags
		registry.emplace<tags::ScheduledTask>(entity);

		return entity;
	}

	auto ScheduledTask::destroy_all(entt::registry& registry) noexcept -> void
	{
		auto& [context] = registry.ctx().get<scheduled_task::Context>();
		context.clear();

		const auto view = registry.view<tags::ScheduledTask>();
		registry.destroy(view.begin(), view.end());
	}
}
