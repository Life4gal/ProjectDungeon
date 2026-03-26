// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/dungeon.hpp>

#include <game/constants.hpp>

#include <components/dungeon.hpp>

#include <systems/room.hpp>

#include <entt/entt.hpp>

namespace pd::systems
{
	auto Dungeon::create(entt::registry& registry, const std::uint32_t level) noexcept -> void
	{
		using namespace components;
		using namespace systems;

		constexpr auto start_position = sf::Vector2u{game::DungeonFloorWidth / 2, game::DungeonFloorHeight / 2};
		const auto count = game::DungeonRoomBaseCount + level * game::DungeonRoomCountGrowthFactor;

		// 创建房间
		Room::create(registry, start_position, count);

		// 保存关卡信息
		registry.ctx().emplace<dungeon::LevelInfo>(level);
	}

	auto Dungeon::destroy(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		// 销毁房间
		Room::destroy(registry);

		// 销毁关卡信息
		registry.ctx().erase<dungeon::LevelInfo>();
	}
}
