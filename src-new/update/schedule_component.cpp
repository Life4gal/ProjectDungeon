// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/schedule_component.hpp>

#include <ranges>

#include <component/schedule_component.hpp>
#include <component/scheduler.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::update
{
	using namespace component;

	namespace
	{
		template<bool Inplace>
		class Invoker final
		{
			static auto invoke(entt::registry& registry, const entt::entity entity, const schedule_component::Components& components) noexcept -> void
			{
				for (const auto [id, name]: std::views::zip(components.ids, components.names))
				{
					// 获取组件池
					auto* storage = registry.storage(id);
					if (storage == nullptr)
					{
						SPDLOG_WARN("找不到指定组件池({}),无法移除组件!", name);

						return;
					}

					// 移除组件(从池中移除该实体)
					if (not storage->remove(entity))
					{
						SPDLOG_WARN("实体(0x{:08x})不存在所需组件({}),无法移除组件!", entt::to_integral(entity), name);

						return;
					}

					//
				}
			}

		public:
			static auto update(entt::registry& registry) noexcept -> void
			{
				if constexpr (Inplace)
				{
					for (const auto view = registry.view<scheduler::Finished, schedule_component::Components>();
					     const auto [entity, components]: view.each())
					{
						invoke(registry, entity, components);
					}
				}
				else
				{
					for (const auto view = registry.view<schedule_component::Scheduler, schedule_component::Components>();
					     const auto [entity, scheduler, components]: view.each())
					{
						if (not registry.valid(scheduler.scheduler))
						{
							invoke(registry, entity, components);
						}
					}
				}
			}
		};
	}

	auto schedule_component(entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		Invoker<false>::update(registry);

		Invoker<true>::update(registry);
	}
}
