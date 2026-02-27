// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/level.hpp>

#include <config/dungeon.hpp>

#include <asset/font.hpp>
#include <asset/texture.hpp>
#include <asset/sound.hpp>
#include <asset/music.hpp>

#include <components/level.hpp>

#include <systems/helper/texture_manager.hpp>
#include <systems/helper/room.hpp>

#include <game/constants.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::systems::helper
{
	auto Level::create(entt::registry& registry, const config::Level& level) noexcept -> entt::entity
	{
		using namespace components;

		// 创建上下文
		registry.ctx().emplace<level::Level>(std::cref(level));
		registry.ctx().emplace<level::Rooms>();

		// 创建资源管理器
		// todo: 在这里创建吗?
		// 如果有改动,别忘了Level::destroy
		{
			registry.ctx().emplace<asset::FontManager>();
			registry.ctx().emplace<asset::TextureManager>();
			registry.ctx().emplace<asset::SoundManager>();
			registry.ctx().emplace<asset::MusicManager>();
		}

		// 加载动画集
		{
			for (const auto& [animation_id, animation]: level.animation_set)
			{
				for (const auto& frame: animation.frames)
				{
					SPDLOG_INFO("加载动画[{}]的纹理[{}]...", animation_id, frame.texture_path);
					[[maybe_unused]] const auto texture_id = TextureManager::load(registry, frame.texture_path);
				}
			}
		}

		// 检查瓦片集
		// todo: 理论上在 config/level.cpp --> from_xxx::load_level 时就应该检查了,这里不需要检查
		{
			const auto do_check_tiles = [&](const std::string_view type, const auto& tile_set) noexcept -> void
			{
				for (const auto& [tile_id, tile]: tile_set)
				{
					if (const auto animation_it = level.animation_set.find(tile.animation_id);
						animation_it == level.animation_set.end())
					{
						SPDLOG_ERROR("{}瓦片纹理[{}]的动画[{}]不存在!", type, tile_id, tile.animation_id);
						// todo: 如何处理?
					}
				}
			};

			// walls
			do_check_tiles("墙壁", level.tile_set.wall_tiles);
			// floors
			do_check_tiles("地板", level.tile_set.floor_tiles);
			// decorations
			do_check_tiles("装饰物", level.tile_set.decoration_tiles);
			// triggers
			do_check_tiles("触发器", level.tile_set.trigger_tiles);
		}

		// 虽然当前关卡没有实体组件,不过也可以先创建
		const auto level_entity = registry.create();

		// 创建初始房间
		if (not create_room(registry, level_entity, level.starting_room))
		{
			SPDLOG_ERROR("关卡[{}]创建初始房间[{}]失败!", level.name, level.starting_room);
		}

		// 切换到初始房间
		if (not switch_room(registry, level_entity, level.starting_room))
		{
			SPDLOG_ERROR("关卡[{}]切换初始房间[{}]失败!", level.name, level.starting_room);
		}

		return level_entity;
	}

	auto Level::destroy(entt::registry& registry, const entt::entity level_entity) noexcept -> void
	{
		using namespace components;

		const auto& level = registry.ctx().get<const level::Level>().level.get();

		// 卸载动画集
		{
			for (const auto& animation: level.animation_set | std::views::values)
			{
				for (const auto& frame: animation.frames)
				{
					TextureManager::unload(registry, frame.texture_path);
				}
			}
		}

		// 卸载瓦片集
		{
			// 
		}

		// 销毁所有房间实体
		const auto& [rooms] = registry.ctx().get<const level::Rooms>();
		for (const auto& [room_id, room_entity]: rooms)
		{
			Room::destroy(registry, room_entity);
			SPDLOG_INFO("销毁关卡[{}]的房间[{}]", level.name, room_id);
		}

		// 清除上下文组件
		registry.ctx().erase<level::Level>();
		registry.ctx().erase<level::Rooms>();

		// 销毁资源管理器
		{
			registry.ctx().erase<asset::FontManager>();
			registry.ctx().erase<asset::TextureManager>();
			registry.ctx().erase<asset::SoundManager>();
			registry.ctx().erase<asset::MusicManager>();
		}

		// 销毁关卡实体
		registry.destroy(level_entity);
	}

	auto Level::create_room(entt::registry& registry, const entt::entity level_entity, const std::string& room_id) noexcept -> bool
	{
		using namespace components;

		std::ignore = level_entity;

		const auto& level = registry.ctx().get<const level::Level>().level.get();
		auto& [rooms] = registry.ctx().get<level::Rooms>();

		if (rooms.contains(room_id))
		{
			SPDLOG_ERROR("关卡[{}]的房间[{}]已创建!", level.name, room_id);
			return false;
		}

		const auto room_it = level.room_set.find(level.starting_room);
		if (room_it == level.room_set.end())
		{
			SPDLOG_ERROR("关卡[{}]创建房间[{}]失败!房间不存在!", level.name, level.starting_room);
			return false;
		}

		const auto& room = room_it->second;

		// 创建房间
		const auto room_entity = Room::create(registry, room, level.animation_set, level.tile_set);

		// 保存房间实体
		rooms.emplace(room_id, room_entity);

		return true;
	}

	auto Level::destroy_room(entt::registry& registry, const entt::entity level_entity, const std::string& room_id) noexcept -> void
	{
		using namespace components;

		std::ignore = level_entity;

		const auto& level = registry.ctx().get<const level::Level>().level.get();
		auto& [rooms] = registry.ctx().get<level::Rooms>();

		const auto room_it = rooms.find(room_id);
		if (room_it == rooms.end())
		{
			SPDLOG_WARN("关卡[{}]销毁房间[{}]失败!房间未创建!", level.name, room_id);
			return;
		}

		const auto room_entity = room_it->second;

		// 销毁房间实体
		Room::destroy(registry, room_entity);

		// 移除房间实体
		rooms.erase(room_it);
	}

	auto Level::switch_room(entt::registry& registry, const entt::entity level_entity, const std::string& room_id) noexcept -> bool
	{
		using namespace components;

		std::ignore = level_entity;

		const auto& level = registry.ctx().get<const level::Level>().level.get();
		auto& [rooms] = registry.ctx().get<level::Rooms>();

		const auto room_it = rooms.find(room_id);
		if (room_it == rooms.end())
		{
			SPDLOG_WARN("关卡[{}]切换房间[{}]失败!房间未创建!", level.name, room_id);
			return false;
		}

		// 如果之前存在房间,则隐藏其内容
		{
			// todo
		}

		// 让新房间内容显示
		{
			// todo
		}

		// 设置当前房间
		registry.ctx().insert_or_assign<level::Room>({room_it->second});

		return true;
	}

	auto Level::is_room_cleared(entt::registry& registry) noexcept -> bool
	{
		using namespace components;

		const auto [room_entity] = registry.ctx().get<const level::Room>();

		return Room::is_cleared(registry, room_entity);
	}

	auto Level::update_room(entt::registry& registry) noexcept -> void
	{
		using namespace components;

		if (is_room_cleared(registry))
		{
			// 如果房间已经清空了,就不需要更新了
			return;
		}

		const auto [room_entity] = registry.ctx().get<const level::Room>();

		// 更新房间状态
		Room::update_enemy_entities(registry, room_entity);
	}
}
