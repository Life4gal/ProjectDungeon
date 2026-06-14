// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <update/schedule_destroy.hpp>

#include <component/schedule_destroy.hpp>
#include <component/scheduler.hpp>
#include <component/tags.hpp>

#include <entt/entt.hpp>

namespace pd::update
{
	using namespace component;

	namespace
	{
		template<bool Inplace>
		class Invoker final
		{
			static auto invoke(entt::registry& registry, const entt::entity entity, const schedule_destroy::Mode mode) noexcept -> void
			{
				if (mode == schedule_destroy::Mode::INSTANCE)
				{
					registry.destroy(entity);
				}
				else
				{
					registry.emplace<state::EntityDead>(entity);
				}
			}

		public:
			static auto update(entt::registry& registry) noexcept -> void
			{
				if constexpr (Inplace)
				{
					for (const auto view = registry.view<scheduler::Finished, const schedule_destroy::Mode>();
					     const auto [entity, mode]: view.each())
					{
						invoke(registry, entity, mode);
					}
				}
				else
				{
					for (const auto view = registry.view<const schedule_destroy::Scheduler, const schedule_destroy::Mode>();
					     const auto [entity, scheduler, mode]: view.each())
					{
						if (not registry.valid(scheduler.scheduler))
						{
							invoke(registry, entity, mode);
						}
					}
				}
			}
		};
	}

	auto schedule_destroy(entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		Invoker<false>::update(registry);

		Invoker<true>::update(registry);
	}
}
