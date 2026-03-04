// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

// 动画集
#include <config/animation.hpp>
// // 字体集
// #include <config/font.hpp>
// // 音效集
// #include <config/sound.hpp>
// // 音乐集
// #include <config/music.hpp>
// 瓦片集
#include <config/tile.hpp>

// 玩家集
#include <config/player.hpp>
// 敌人集
#include <config/enemy.hpp>

// 房间集
#include <config/room.hpp>

namespace pd::config
{
	class Level final
	{
	public:
		// 关卡名称
		std::string name;

		// 动画集
		// todo: 动画集是dungeon通用还是每个level单独加载?
		AnimationSet animation_set;

		// todo: 字体集?音效集?音乐集?

		// 瓦片集
		// todo: 瓦片集是dungeon通用还是每个level单独加载?
		TileSet tile_set;

		// todo: 武器集

		// 玩家配置
		PlayerSet player_set;
		// 本关使用的玩家ID
		std::string player_id;

		// 敌人配置(暂未使用,但需要提前加载验证)
		EnemySet enemy_set;

		// 关卡包含的房间集合
		RoomSet room_set;
		// 初始房间ID
		std::string starting_room;
	};

	// 关卡集
	// 关卡ID -> 关卡配置
	// JSON:
	// { "levels": "/path/to/levels.json" }
	// { "levels": { "level-id-1": { Level }, "level-id-2": { Level }, ... } }
	class LevelSet final : public Set<Level>
	{
	public:
		using Set::Set;
	};

	[[nodiscard]] auto load_level_from_json(Level& level, const ConfigReader::json_format& json) noexcept -> bool;
	[[nodiscard]] auto load_level_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<Level>;

	[[nodiscard]] auto load_level_set_from_json(LevelSet& level_set, const ConfigReader::json_format& json) noexcept -> bool;
	[[nodiscard]] auto load_level_set_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<LevelSet>;
}
