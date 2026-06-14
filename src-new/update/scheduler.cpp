// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/scheduler.hpp>

#include <component/scheduler.hpp>
#include <component/transform.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::update
{
	using namespace component;

	namespace
	{
		template<bool InPlace>
		class Invoker final
		{
			static auto invoke(entt::registry& registry, const entt::entity entity) noexcept -> void
			{
				if constexpr (InPlace)
				{
					// 所有者附加该组件
					registry.emplace<scheduler::Finished>(entity);
				}
				else
				{
					// 调度器直接销毁
					registry.destroy(entity);
				}
			}

		public:
			static auto time(entt::registry& registry, const sf::Time delta) noexcept -> void
			{
				for (const auto view = registry.view<const scheduler::Duration, scheduler::Elapsed>();
				     const auto [entity, duration, elapsed]: view.each())
				{
					elapsed.elapsed += delta;
					if (elapsed.elapsed >= duration.duration)
					{
						invoke(registry, entity);
					}
				}
			}

			static auto distance(entt::registry& registry) noexcept -> void
			{
				const auto check = [](const scheduler::DistanceSquared& distance_squared, const scheduler::AnchorPosition& anchor_position, const transform::Position& position) noexcept -> bool
				{
					const auto diff = position.position - anchor_position.position;
					const auto diff_squared = diff.lengthSquared();

					return diff_squared >= distance_squared.value;
				};

				if constexpr (InPlace)
				{
					for (const auto view = registry.view<const scheduler::DistanceSquared, const scheduler::AnchorPosition, const transform::Position>();
					     const auto [entity, distance_squared, anchor_position, position]: view.each())
					{
						if (check(distance_squared, anchor_position, position))
						{
							invoke(registry, entity);
						}
					}
				}
				else
				{
					for (const auto view = registry.view<const scheduler::Owner, const scheduler::DistanceSquared, const scheduler::AnchorPosition>();
					     const auto [entity, owner, distance_squared, anchor_position]: view.each())
					{
						const auto* position = registry.try_get<transform::Position>(owner.owner);
						if (position == nullptr)
						{
							// 创建时必定存在该组件
							// 如果此时不存在该组件则说明transform::Position被移除
							SPDLOG_INFO("因实体(0x{:08x})的transform::Position组件被移除,终止调度器!", entt::to_integral(owner.owner));

							// TODO: 这实际上产生的效果就是调度器执行完毕 :(
							invoke(registry, entity);
							continue;
						}

						if (check(distance_squared, anchor_position, *position))
						{
							invoke(registry, entity);
						}
					}
				}
			}
		};
	}

	auto scheduler(entt::registry& registry, const sf::Time delta) noexcept -> void
	{
		Invoker<false>::time(registry, delta);
		Invoker<false>::distance(registry);

		Invoker<true>::time(registry, delta);
		Invoker<true>::distance(registry);
	}
}
