// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/scheduled_task.hpp>

#include <entt/entity/fwd.hpp>
#include <entt/core/type_info.hpp>

namespace pd::factory
{
	class ScheduledTask final
	{
	public:
		// TODO: 把这两接口移动到合适的位置

		template<typename... Components>
		[[nodiscard]] constexpr static auto ids_of() noexcept -> blueprint::ScheduledTask::RemoveComponent::ids_type
		{
			return blueprint::ScheduledTask::RemoveComponent::ids_type{entt::type_hash<Components>::value()...};
		}

		template<typename... Components>
		[[nodiscard]] constexpr static auto names_of() noexcept -> std::vector<std::string_view>
		{
			return std::vector<std::string_view>{entt::type_name<Components>::value()...};
		}

	private:
		static auto spawn(
			entt::registry& registry,
			entt::entity owner,
			const blueprint::ScheduledTask::TimeCycle& time_cycle,
			blueprint::ScheduledTask::RemoveComponent::ids_type ids,
			std::vector<std::string_view> names
		) noexcept -> entt::entity;

		static auto spawn(
			entt::registry& registry,
			entt::entity owner,
			const blueprint::ScheduledTask::DistanceCycle& distance_cycle,
			blueprint::ScheduledTask::RemoveComponent::ids_type ids,
			std::vector<std::string_view> names
		) noexcept -> entt::entity;

	public:
		// 时间
		// 不指定组件 --> 销毁实体
		template<typename... Components>
		static auto spawn(
			entt::registry& registry,
			const entt::entity owner,
			const blueprint::ScheduledTask::TimeCycle& time_cycle
		) noexcept -> entt::entity
		{
			return spawn(registry, owner, time_cycle, ids_of<Components...>(), names_of<Components...>());
		}

		// 距离
		// 不指定组件 --> 销毁实体
		template<typename... Components>
		static auto spawn(
			entt::registry& registry,
			const entt::entity owner,
			const blueprint::ScheduledTask::DistanceCycle& distance_cycle
		) noexcept -> entt::entity
		{
			return spawn(registry, owner, distance_cycle, ids_of<Components...>(), names_of<Components...>());
		}

		// 任一
		// 不指定组件 --> 销毁实体
		template<typename... Components>
		static auto spawn(
			entt::registry& registry,
			const entt::entity owner,
			const blueprint::ScheduledTask::scheduled_type& scheduled
		) noexcept -> entt::entity
		{
			return std::visit(
				[&](const auto& s) noexcept -> entt::entity
				{
					return spawn<Components...>(registry, owner, s);
				},
				scheduled
			);
		}

		static auto destroy_all(entt::registry& registry) noexcept -> void;
	};
}
