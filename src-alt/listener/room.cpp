// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/room.hpp>

#include <manager/event.hpp>

#include <events/room.hpp>

#include <components/tags.hpp>
#include <components/room.hpp>

#include <accessor/physics.hpp>

#include <entt/entt.hpp>

namespace pd::listener
{
	namespace
	{
		namespace door
		{
			auto do_disable(entt::registry& registry, const entt::entity entity) noexcept -> void
			{
				using namespace components;

				registry.emplace_or_replace<tags::disabled>(entity);

				// FIXME: deattach vs disable
				accessor::Physics::disable(registry, entity);
			}

			auto do_enable(entt::registry& registry, const entt::entity entity) noexcept -> void
			{
				using namespace components;

				registry.remove<tags::disabled>(entity);

				// FIXME: attach vs enable
				accessor::Physics::enable(registry, entity);
			}

			namespace ed = events::room::door;
			namespace cd = components::room::door;

			auto on_disable(entt::registry& registry, const ed::Disable& event) noexcept -> void
			{
				do_disable(registry, event.door);
			}

			auto on_enable(entt::registry& registry, const ed::Enable& event) noexcept -> void
			{
				do_enable(registry, event.door);
			}

			auto on_disable_all(entt::registry& registry, const ed::DisableAll& event) noexcept -> void
			{
				for (const auto door: event.doors)
				{
					do_disable(registry, door);
				}
			}

			auto on_enable_all(entt::registry& registry, const ed::EnableAll& event) noexcept -> void
			{
				for (const auto door: event.doors)
				{
					do_enable(registry, door);
				}
			}

			auto on_open_request(entt::registry& registry, [[maybe_unused]] const ed::OpenRequest& event) noexcept -> void
			{
				using namespace components;

				for (const auto view = registry.view<cd::State>(entt::exclude<tags::disabled>); const auto [entity, state]: view.each())
				{
					if (state == cd::State::OPENING or state == cd::State::OPENED)
					{
						continue;
					}

					state = cd::State::OPENING;
					registry.emplace_or_replace<cd::Timer>(entity, sf::Time::Zero);
				}
			}

			auto on_close_request(entt::registry& registry, [[maybe_unused]] const ed::CloseRequest& event) noexcept -> void
			{
				using namespace components;

				for (const auto view = registry.view<cd::State>(entt::exclude<tags::disabled>); const auto [entity, state]: view.each())
				{
					if (state == cd::State::CLOSING or state == cd::State::CLOSED)
					{
						continue;
					}

					state = cd::State::CLOSING;
					registry.emplace_or_replace<cd::Timer>(entity, sf::Time::Zero);
				}
			}

			auto on_contact(entt::registry& registry, [[maybe_unused]] const ed::Contacted& event) noexcept -> void
			{
				// 门本身应该是不在乎这个事件的
				// 或者说接触事件不在这里处理
				// todo: 门应该在乎这个事件,只是我们可能需要足够的信息才能处理?
			}

			auto on_contact_sensor(entt::registry& registry, [[maybe_unused]] const ed::SensorContacted& event) noexcept -> void
			{
				// 门本身应该是不在乎这个事件的
				// 或者说接触事件不在这里处理
				// todo: 门应该在乎这个事件,只是我们可能需要足够的信息才能处理?
			}

			auto subscribe_all(entt::registry& registry) noexcept -> void
			{
				using namespace manager;
				using namespace events;

				Event::subscribe<ed::Disable, &on_disable>(registry);
				Event::subscribe<ed::Enable, &on_enable>(registry);
				Event::subscribe<ed::DisableAll, &on_disable_all>(registry);
				Event::subscribe<ed::EnableAll, &on_enable_all>(registry);
				Event::subscribe<ed::OpenRequest, &on_open_request>(registry);
				Event::subscribe<ed::CloseRequest, &on_close_request>(registry);
				Event::subscribe<ed::Contacted, &on_contact>(registry);
				Event::subscribe<ed::SensorContacted, &on_contact_sensor>(registry);
			}

			auto unsubscribe_all(entt::registry& registry) noexcept -> void
			{
				using namespace manager;
				using namespace events;

				Event::unsubscribe<ed::Disable, &on_disable>(registry);
				Event::unsubscribe<ed::Enable, &on_enable>(registry);
				Event::unsubscribe<ed::DisableAll, &on_disable_all>(registry);
				Event::unsubscribe<ed::EnableAll, &on_enable_all>(registry);
				Event::unsubscribe<ed::OpenRequest, &on_open_request>(registry);
				Event::unsubscribe<ed::CloseRequest, &on_close_request>(registry);
				Event::unsubscribe<ed::Contacted, &on_contact>(registry);
				Event::unsubscribe<ed::SensorContacted, &on_contact_sensor>(registry);
			}
		}

		namespace chest
		{
			namespace ec = events::room::chest;
			namespace cc = components::room::chest;

			auto on_disable(entt::registry& registry, const ec::Disable& event) noexcept -> void
			{
				using namespace components;

				registry.emplace_or_replace<tags::disabled>(event.chest);
			}

			auto on_enable(entt::registry& registry, const ec::Enable& event) noexcept -> void
			{
				using namespace components;

				registry.remove<tags::disabled>(event.chest);
			}

			auto on_disable_all(entt::registry& registry, const ec::DisableAll& event) noexcept -> void
			{
				using namespace components;

				for (const auto chest: event.chests)
				{
					registry.emplace_or_replace<tags::disabled>(chest);
				}
			}

			auto on_enable_all(entt::registry& registry, const ec::EnableAll& event) noexcept -> void
			{
				using namespace components;

				for (const auto chest: event.chests)
				{
					registry.remove<tags::disabled>(chest);
				}
			}

			auto on_contact(entt::registry& registry, const ec::Contacted& event) noexcept -> void
			{
				// todo
			}

			auto subscribe_all(entt::registry& registry) noexcept -> void
			{
				using namespace manager;
				using namespace events;

				Event::subscribe<ec::Disable, &on_disable>(registry);
				Event::subscribe<ec::Enable, &on_enable>(registry);
				Event::subscribe<ec::DisableAll, &on_disable_all>(registry);
				Event::subscribe<ec::EnableAll, &on_enable_all>(registry);
				Event::subscribe<ec::Contacted, &on_contact>(registry);
			}

			auto unsubscribe_all(entt::registry& registry) noexcept -> void
			{
				using namespace manager;
				using namespace events;

				Event::unsubscribe<ec::Disable, &on_disable>(registry);
				Event::unsubscribe<ec::Enable, &on_enable>(registry);
				Event::unsubscribe<ec::DisableAll, &on_disable_all>(registry);
				Event::unsubscribe<ec::EnableAll, &on_enable_all>(registry);
				Event::unsubscribe<ec::Contacted, &on_contact>(registry);
			}
		}

		namespace er = events::room;
		namespace cr = components::room;

		auto on_enter_room([[maybe_unused]] entt::registry& registry, const er::Entered& event) noexcept -> void
		{
			using namespace manager;

			// 激活当前房间的实体
			if (const auto* doors = registry.try_get<cr::Doors>(event.room);
				doors != nullptr and not doors->entities.empty())
			{
				Event::enqueue(er::door::EnableAll{doors->entities});
			}
			if (const auto* items = registry.try_get<cr::Items>(event.room);
				items != nullptr and not items->entities.empty())
			{
				Event::enqueue(er::chest::EnableAll{items->entities});
			}
			if (const auto* chests = registry.try_get<cr::Chests>(event.room);
				chests != nullptr and not chests->entities.empty())
			{
				Event::enqueue(er::chest::EnableAll{chests->entities});
			}
			if (const auto* blood_stains = registry.try_get<cr::BloodStains>(event.room);
				blood_stains != nullptr and not blood_stains->entities.empty())
			{
				Event::enqueue(er::blood_stain::EnableAll{blood_stains->entities});
			}
			if (const auto* corpses = registry.try_get<cr::Corpses>(event.room);
				corpses != nullptr and not corpses->entities.empty())
			{
				Event::enqueue(er::corpse::EnableAll{corpses->entities});
			}
			if (const auto* destroyable = registry.try_get<cr::Destroyable>(event.room);
				destroyable != nullptr and not destroyable->entities.empty())
			{
				Event::enqueue(er::destroyable::EnableAll{destroyable->entities});
			}

			// 如果房间未清除
			if (not registry.all_of<cr::Cleared>(event.room))
			{
				//  请求关闭所有门(如果可以关闭)
				Event::enqueue(er::door::CloseRequest{});
			}
		}

		auto on_clear_room([[maybe_unused]] entt::registry& registry, [[maybe_unused]] const er::Cleared& event) noexcept -> void
		{
			using namespace manager;

			// 请求开启所有门(如果可以开启)
			Event::enqueue(er::door::OpenRequest{});
		}

		auto on_leave_room([[maybe_unused]] entt::registry& registry, const er::Left& event) noexcept -> void
		{
			using namespace manager;

			// 禁用当前房间的实体
			if (const auto* doors = registry.try_get<cr::Doors>(event.room);
				doors != nullptr and not doors->entities.empty())
			{
				Event::enqueue(er::door::EnableAll{doors->entities});
			}
			if (const auto* items = registry.try_get<cr::Items>(event.room);
				items != nullptr and not items->entities.empty())
			{
				Event::enqueue(er::chest::EnableAll{items->entities});
			}
			if (const auto* chests = registry.try_get<cr::Chests>(event.room);
				chests != nullptr and not chests->entities.empty())
			{
				Event::enqueue(er::chest::EnableAll{chests->entities});
			}
			if (const auto* blood_stains = registry.try_get<cr::BloodStains>(event.room);
				blood_stains != nullptr and not blood_stains->entities.empty())
			{
				Event::enqueue(er::blood_stain::EnableAll{blood_stains->entities});
			}
			if (const auto* corpses = registry.try_get<cr::Corpses>(event.room);
				corpses != nullptr and not corpses->entities.empty())
			{
				Event::enqueue(er::corpse::EnableAll{corpses->entities});
			}
			if (const auto* destroyable = registry.try_get<cr::Destroyable>(event.room);
				destroyable != nullptr and not destroyable->entities.empty())
			{
				Event::enqueue(er::destroyable::EnableAll{destroyable->entities});
			}

			// 理论上不可能没有清理
			// if (not registry.all_of<cr::Cleared>(event.room)) {}
		}
	}

	auto Room::subscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;

		// door
		door::subscribe_all(registry);
		// chest
		chest::subscribe_all(registry);

		// room
		Event::subscribe<er::Entered, &on_enter_room>(registry);
		Event::subscribe<er::Cleared, &on_clear_room>(registry);
		Event::subscribe<er::Left, &on_leave_room>(registry);
	}

	auto Room::unsubscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;

		// door
		door::unsubscribe_all(registry);
		// chest
		chest::unsubscribe_all(registry);

		// room
		Event::unsubscribe<er::Entered, &on_enter_room>(registry);
		Event::unsubscribe<er::Cleared, &on_clear_room>(registry);
		Event::unsubscribe<er::Left, &on_leave_room>(registry);
	}
}
