// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/floor.hpp>

#include <manager/event.hpp>

#include <events/floor.hpp>
#include <events/room.hpp>

#include <components/floor.hpp>
#include <components/room.hpp>

#include <entt/entt.hpp>

namespace pd::listener
{
	namespace
	{
		namespace ef = events::floor;
		namespace cf = components::floor;

		auto on_generate_request(entt::registry& registry, const ef::GenerateRequest& event) noexcept -> void
		{
			const auto layout = blueprint::Floor::generate({.x = event.start_x, .y = event.start_y}, event.count);

			registry.ctx().insert_or_assign(cf::Layout{.layout = layout});
			registry.ctx().insert_or_assign(cf::RoomCount{event.count});
			registry.ctx().insert_or_assign(cf::CurrentRoom{.x = event.start_x, .y = event.start_y});
		}

		auto on_destroy_request(entt::registry& registry, [[maybe_unused]] const ef::DestroyRequest& event) noexcept -> void
		{
			registry.ctx().erase<cf::Layout>();
			registry.ctx().erase<cf::RoomCount>();
			registry.ctx().erase<cf::CurrentRoom>();
		}

		auto on_enter_room(entt::registry& registry, const ef::Entered& event) noexcept -> void
		{
			using namespace manager;

			namespace er = events::room;
			namespace cr = components::room;

			// todo: room不需要(也不应该有)坐标组件,但是在我们有更好的获取目标房间实体的方法之前,先这么做吧 :)
			for (const auto view = registry.view<const cr::Position>();
			     const auto [entity, position]: view.each())
			{
				if (position.x == event.x and position.y == event.y)
				{
					Event::enqueue(er::Entered{.room = entity});
					break;
				}
			}
		}

		auto on_clear_room(entt::registry& registry, const ef::Cleared& event) noexcept -> void
		{
			using namespace manager;

			namespace er = events::room;
			namespace cr = components::room;

			// todo: room不需要(也不应该有)坐标组件,但是在我们有更好的获取目标房间实体的方法之前,先这么做吧 :)
			for (const auto view = registry.view<const cr::Position>();
			     const auto [entity, position]: view.each())
			{
				if (position.x == event.x and position.y == event.y)
				{
					Event::enqueue(er::Cleared{.room = entity});
					break;
				}
			}
		}

		auto on_leave_room(entt::registry& registry, const ef::Left& event) noexcept -> void
		{
			using namespace manager;

			namespace er = events::room;
			namespace cr = components::room;

			// todo: room不需要(也不应该有)坐标组件,但是在我们有更好的获取目标房间实体的方法之前,先这么做吧 :)
			for (const auto view = registry.view<const cr::Position>();
			     const auto [entity, position]: view.each())
			{
				if (position.x == event.x and position.y == event.y)
				{
					Event::enqueue(er::Left{.room = entity});
					break;
				}
			}
		}
	}

	auto Floor::subscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;

		Event::subscribe<ef::GenerateRequest, &on_generate_request>(registry);
		Event::subscribe<ef::DestroyRequest, &on_destroy_request>(registry);
		Event::subscribe<ef::Entered, &on_enter_room>(registry);
		Event::subscribe<ef::Cleared, &on_clear_room>(registry);
		Event::subscribe<ef::Left, &on_leave_room>(registry);
	}

	auto Floor::unsubscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;

		Event::unsubscribe<ef::GenerateRequest, &on_generate_request>(registry);
		Event::unsubscribe<ef::DestroyRequest, &on_destroy_request>(registry);
		Event::unsubscribe<ef::Entered, &on_enter_room>(registry);
		Event::unsubscribe<ef::Cleared, &on_clear_room>(registry);
		Event::unsubscribe<ef::Left, &on_leave_room>(registry);
	}
}
