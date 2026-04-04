// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/dungeon.hpp>

#include <manager/event.hpp>

#include <game/constants.hpp>

#include <components/dungeon.hpp>

#include <events/floor.hpp>

#include <systems/floor.hpp>

#include <entt/entt.hpp>

namespace pd::systems
{
	auto Dungeon::subscribe_events(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::subscribe<dungeon::Go, &on_go>(registry);
	}

	auto Dungeon::unsubscribe_events(entt::registry& registry) noexcept -> void
	{
		using namespace manager;
		using namespace events;

		Event::unsubscribe<dungeon::Go, &on_go>(registry);
	}

	auto Dungeon::on_go(entt::registry& registry, const events::dungeon::Go& event) noexcept -> void
	{
		using namespace manager;
		using namespace components;
		using namespace systems;

		// 保存信息
		registry.ctx().insert_or_assign<dungeon::Info>(dungeon::Info{.level = event.level});
		
		const auto count = game::FloorRoomBaseCount + event.level * game::FloorRoomCountGrowthFactor;
		
		// 创建指定层级的地牢布局
		Event::enqueue(events::floor::GenerateRequest{.count = count, .start_x = event.x, .start_y = event.y});
		// 进入房间
		Event::enqueue(events::floor::Entered{.x = event.x, .y = event.y});
	}

	auto Dungeon::create(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		registry.ctx().emplace<dungeon::Info>(dungeon::Info{.level = 0});
	}

	auto Dungeon::destroy(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		// 销毁当前层级(如果有)
		Floor::destroy(registry);

		// 销毁信息
		registry.ctx().erase<dungeon::Info>();
	}
}
