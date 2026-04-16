// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/dungeon.hpp>

#include <game/constants.hpp>

#include <manager/event.hpp>

#include <events/dungeon.hpp>
#include <events/floor.hpp>

#include <components/dungeon.hpp>

#include <entt/entt.hpp>

namespace pd::listener
{
	namespace
	{
		namespace ed = events::dungeon;
		namespace cd = components::dungeon;

		auto on_go(entt::registry& registry, const ed::Go& event) noexcept -> void
		{
			using namespace manager;

			// 保存信息
			registry.ctx().insert_or_assign(cd::Info{.level = event.level});

			// 创建指定层级的地牢布局
			const auto count = game::FloorRoomBaseCount + event.level * game::FloorRoomCountGrowthFactor;
			Event::enqueue(events::floor::GenerateRequest{.count = count, .start_x = event.x, .start_y = event.y});
			// 进入房间
			Event::enqueue(events::floor::Entered{.x = event.x, .y = event.y});
		}

		auto on_exit(entt::registry& registry, [[maybe_unused]] const ed::Exit& event) noexcept -> void
		{
			using namespace manager;

			// 销毁信息
			registry.ctx().erase<cd::Info>();

			// 销毁楼层
			Event::enqueue(events::floor::DestroyRequest{});
		}
	}

	auto Dungeon::subscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;

		Event::subscribe<ed::Go, &on_go>(registry);
		Event::subscribe<ed::Exit, &on_exit>(registry);
	}

	auto Dungeon::unsubscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;

		Event::unsubscribe<ed::Go, &on_go>(registry);
		Event::unsubscribe<ed::Exit, &on_exit>(registry);
	}
}
