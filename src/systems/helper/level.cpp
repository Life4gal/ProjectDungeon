// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/level.hpp>

#include <config/dungeon.hpp>

#include <components/entity/level.hpp>

#include <systems/helper/texture_manager.hpp>
#include <systems/helper/room.hpp>

#include <game/constants.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::systems::helper
{
	auto Level::load_level(entt::registry& registry, const config::Level& level, const config::TileSet& tileset) noexcept -> bool
	{
		using namespace components;

		// 卸载之前的关卡
		unload_level(registry);

		auto& [current_level] = registry.ctx().emplace<level::CurrentLevel>(level);
		auto& [current_tileset] = registry.ctx().emplace<level::CurrentTileSet>(tileset);

		// 加载瓦片集中的所有纹理
		for (const auto& [tile_id, tile]: current_tileset)
		{
			[[maybe_unused]] const auto texture_id = TextureManager::load(registry, tile.texture_path);
			SPDLOG_INFO("加载瓦片纹理[{}]:{}", tile_id, tile.texture_path);
		}

		// 加载初始房间
		if (not switch_room(registry, current_level.starting_room_id))
		{
			SPDLOG_ERROR("关卡[{}]加载初始房间失败", current_level.id);
			return false;
		}

		SPDLOG_INFO("加载关卡[{}]成功", current_level.id);
		return true;
	}

	auto Level::unload_level(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		if (not registry.ctx().contains<level::CurrentLevel>())
		{
			return;
		}

		const auto [room_entity] = registry.ctx().get<const level::CurrentRoom>();

		Room::destroy(registry, room_entity);

		registry.ctx().erase<level::CurrentLevel>();
		registry.ctx().erase<level::CurrentTileSet>();
		registry.ctx().erase<level::CurrentRoom>();
		registry.ctx().erase<level::RoomCleared>();
		registry.ctx().erase<level::RoomOffset>();
	}

	auto Level::switch_room(entt::registry& registry, const std::string& room_id) noexcept -> bool
	{
		using namespace components;

		const auto& [current_level] = registry.ctx().get<const level::CurrentLevel>();
		const auto& [current_tileset] = registry.ctx().get<const level::CurrentTileSet>();

		// 查找目标房间
		const auto room_it = current_level.rooms.find(room_id);
		if (room_it == current_level.rooms.end())
		{
			SPDLOG_ERROR("关卡[{}]中找不到房间:{}", current_level.id, room_id);
			return false;
		}

		const auto& room = room_it->second;

		// 计算房间偏移,使房间居中显示
		const auto room_width = static_cast<float>(room.width * room.tile_size);
		const auto room_height = static_cast<float>(room.height * room.tile_size);

		// 计算偏移使房间居中
		const auto room_offset_x = (static_cast<float>(Constant::window_width) - room_width) * 0.5f;
		const auto room_offset_y = (static_cast<float>(Constant::window_height) - room_height) * 0.5f;
		registry.ctx().emplace<level::RoomOffset>(room_offset_x, room_offset_y);

		// 生成新房间
		const auto room_entity = Room::create(registry, room, current_tileset, {room_offset_x, room_offset_y});
		registry.ctx().emplace<level::CurrentRoom>(room_entity);

		// todo: 根据房间类型设置房间状态

		return true;
	}

	auto Level::is_room_cleared(entt::registry& registry) noexcept -> bool
	{
		using namespace components;

		return registry.ctx().contains<level::RoomCleared>();
	}

	auto Level::check_room(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		if (const auto [room_entity] = registry.ctx().get<const level::CurrentRoom>();
			Room::update_enemy_entities(registry, room_entity))
		{
			Room::unlock_doors(registry, room_entity);
		}
	}

	auto Level::get_room_id(entt::registry& registry) noexcept -> std::string_view
	{
		using namespace components;

		const auto [room_entity] = registry.ctx().get<const level::CurrentRoom>();

		return Room::id(registry, room_entity);
	}
}
