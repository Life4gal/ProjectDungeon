// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/dungeon.hpp>

#include <manager/event.hpp>

#include <game/constants.hpp>

#include <components/dungeon.hpp>

#include <entt/entt.hpp>

namespace pd::listener
{
	auto Dungeon::subscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::subscribe<dungeon::Go, &on_go>(registry);
		Event::subscribe<dungeon::Exit, &Dungeon::on_exit>(registry);
	}

	auto Dungeon::unsubscribe(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::unsubscribe<dungeon::Go, &on_go>(registry);
		Event::unsubscribe<dungeon::Exit, &Dungeon::on_exit>(registry);
	}

	auto Dungeon::on_go(entt::registry& registry, const events::dungeon::Go& event) noexcept -> void
	{
		using namespace manager;
		// using namespace events;
		using namespace components;

		// 保存信息
		registry.ctx().insert_or_assign<dungeon::Info>(dungeon::Info{.level = event.level});

		// 创建指定层级的地牢布局
		const auto count = game::FloorRoomBaseCount + event.level * game::FloorRoomCountGrowthFactor;
		Event::enqueue(events::floor::GenerateRequest{.count = count, .start_x = event.x, .start_y = event.y});
		// 进入房间
		Event::enqueue(events::floor::Entered{.x = event.x, .y = event.y});
	}

	auto Dungeon::on_exit(entt::registry& registry, [[maybe_unused]] const events::dungeon::Exit& event) noexcept -> void
	{
		using namespace manager;
		// using namespace events;
		using namespace components;

		// 销毁信息
		registry.ctx().erase<dungeon::Info>();

		// 销毁楼层
		Event::enqueue(events::floor::DestroyRequest{});
	}
}
