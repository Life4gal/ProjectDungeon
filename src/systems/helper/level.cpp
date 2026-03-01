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

#include <components/tags.hpp>
#include <components/level.hpp>
#include <components/room.hpp>

#include <systems/helper/texture_manager.hpp>
#include <systems/helper/room.hpp>

#include <prometheus/platform/os.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::systems::helper
{
	auto Level::create(entt::registry& registry, const config::Level& level) noexcept -> entt::entity
	{
		using namespace components;

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

		const auto entity = registry.create();

		registry.emplace<level::Level>(entity, std::cref(level));
		registry.emplace<level::Rooms>(entity);
		registry.emplace<level::CollectedKeys>(entity);

		// 创建初始房间
		const auto room_entity = create_room(registry, entity, level.starting_room);
		if (room_entity == entt::null)
		{
			return entt::null;
		}

		// 理论上应该由switch_room设置该组件
		// 如果先设置改组件房间我们join :)
		registry.emplace<level::Room>(entity, room_entity);

		return entity;
	}

	auto Level::destroy(entt::registry& registry, const entt::entity level_entity) noexcept -> void
	{
		using namespace components;

		const auto& level = registry.get<const level::Level>(level_entity).level.get();
		const auto& [rooms] = registry.get<const level::Rooms>(level_entity);

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
		for (const auto& [room_id, room_entity]: rooms)
		{
			Room::destroy(registry, room_entity);
			SPDLOG_INFO("销毁关卡[{}]的房间[{}]", level.name, room_id);
		}

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

	auto Level::join(entt::registry& registry, const entt::entity level_entity, const entt::entity player_entity) noexcept -> bool
	{
		using namespace components;

		if (not registry.all_of<tags::player>(player_entity))
		{
			// 应该不会出这么低级的错误吧 :)
			return false;
		}

		const auto [room_entity] = registry.get<const level::Room>(level_entity);

		// 切换到初始房间
		switch_room(registry, level_entity, entt::null, room_entity, player_entity);
		return true;
	}

	auto Level::on_update(entt::registry& registry, const entt::entity level_entity) noexcept -> void
	{
		using namespace components;

		const auto [room_entity] = registry.get<const level::Room>(level_entity);

		Room::on_update(registry, room_entity);
	}

	auto Level::on_trigger_contact(entt::registry& registry, const entt::entity level_entity, const entt::entity trigger_entity, const entt::entity other_entity) noexcept -> void
	{
		using namespace components;

		std::ignore = registry;
		std::ignore = level_entity;
		std::ignore = trigger_entity;
		std::ignore = other_entity;
	}

	auto Level::on_key_contact(entt::registry& registry, const entt::entity level_entity, const entt::entity key_entity, const entt::entity other_entity) noexcept -> void
	{
		using namespace components;

		// 如果不是玩家接触了钥匙,则不处理
		if (not registry.all_of<tags::player>(other_entity))
		{
			return;
		}

		const auto [room_entity] = registry.get<const level::Room>(level_entity);
		auto& [collected_keys] = registry.get<level::CollectedKeys>(level_entity);
		const auto& [key_name] = registry.get<const key::Name>(key_entity);

		collected_keys.push_back(key_name);

		// 销毁钥匙
		Room::on_key_collected(registry, room_entity, key_entity);
	}

	auto Level::on_locked_door_contact(entt::registry& registry, const entt::entity level_entity, const entt::entity door_entity, const entt::entity other_entity) noexcept -> void
	{
		using namespace components;

		// 如果不是玩家接触了门,则不处理
		if (not registry.all_of<tags::player>(other_entity))
		{
			return;
		}

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<door::Locked>(door_entity));

		// 目前没什么需要做的
		// 也许我们可以显示提示信息,例如要求玩家拥有对应的钥匙?
		std::ignore = level_entity;
		std::ignore = door_entity;

		SPDLOG_INFO("玩家接触锁定的门");
	}

	auto Level::on_unlocked_door_contact(entt::registry& registry, const entt::entity level_entity, const entt::entity door_entity, const entt::entity other_entity) noexcept -> void
	{
		using namespace components;

		// 如果不是玩家接触了门,则不处理
		// 理论上只有玩家能接触到开启的门
		if (not registry.all_of<tags::player>(other_entity))
		{
			return;
		}

		PROMETHEUS_PLATFORM_ASSUME(not registry.all_of<door::Locked>(door_entity));

		const auto& [rooms] = registry.get<const level::Rooms>(level_entity);
		const auto [this_room_entity] = registry.get<const level::Room>(level_entity);
		const auto& [target_room_id] = registry.get<const door::TargetRoom>(door_entity);

		const auto target_room_entity = [&] noexcept -> entt::entity
		{
			// 检查目标房间是否已创建
			if (const auto room_it = rooms.find(target_room_id);
				room_it != rooms.end())
			{
				return room_it->second;
			}

			// 创建房间
			return create_room(registry, level_entity, target_room_id);
		}();

		// todo: 如果create_room失败怎么办?
		if (target_room_entity == entt::null)
		{
			return;
		}

		// 进入目标房间
		switch_room(registry, level_entity, this_room_entity, target_room_entity, other_entity);
	}

	auto Level::create_room(entt::registry& registry, const entt::entity level_entity, const std::string& room_id) noexcept -> entt::entity
	{
		using namespace components;

		const auto& level = registry.get<const level::Level>(level_entity).level.get();
		auto& [rooms] = registry.get<level::Rooms>(level_entity);

		if (rooms.contains(room_id))
		{
			SPDLOG_ERROR("关卡[{}]的房间[{}]已创建!", level.name, room_id);
			return entt::null;
		}

		const auto room_it = level.room_set.find(room_id);
		if (room_it == level.room_set.end())
		{
			SPDLOG_ERROR("关卡[{}]创建房间[{}]失败!房间不存在!", level.name, room_id);
			return entt::null;
		}

		const auto& room = room_it->second;

		// 创建房间
		const auto room_entity = Room::create(registry, room, level.animation_set, level.tile_set);

		// 保存房间实体
		rooms.emplace(room_id, room_entity);

		return room_entity;
	}

	// auto Level::destroy_room(entt::registry& registry, const entt::entity level_entity, const std::string& room_id) noexcept -> void
	// {
	// 	using namespace components;
	//
	// 	const auto& level = registry.get<const level::Level>(level_entity).level.get();
	// 	auto& [rooms] = registry.get<level::Rooms>(level_entity);
	//
	// 	const auto room_it = rooms.find(room_id);
	// 	if (room_it == rooms.end())
	// 	{
	// 		SPDLOG_WARN("关卡[{}]销毁房间[{}]失败!房间未创建!", level.name, room_id);
	// 		return;
	// 	}
	//
	// 	const auto room_entity = room_it->second;
	//
	// 	// 销毁房间实体
	// 	Room::destroy(registry, room_entity);
	//
	// 	// 移除房间实体
	// 	rooms.erase(room_it);
	// }

	auto Level::switch_room(
		entt::registry& registry,
		const entt::entity level_entity,
		const entt::entity this_room_entity,
		const entt::entity target_room_entity,
		const entt::entity player_entity
	) noexcept -> void
	{
		using namespace components;

		const auto& [collected_keys] = registry.get<const level::CollectedKeys>(level_entity);

		// 离开当前房间
		// 当且仅当目标房间是初始房间时,this_room_entity才会是entt::null
		if (this_room_entity == entt::null)
		{
			const auto& target_room = registry.get<const room::Room>(target_room_entity).room.get();
			PROMETHEUS_PLATFORM_ASSUME(target_room.type == config::RoomType::STARTING);
		}
		else
		{
			Room::on_exit(registry, this_room_entity, player_entity);
		}

		// 进入目标房间
		Room::on_enter(registry, target_room_entity, player_entity, collected_keys);

		// 设置当前房间
		registry.emplace_or_replace<level::Room>(level_entity, target_room_entity);
	}
}
