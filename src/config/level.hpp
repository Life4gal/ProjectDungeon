// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <config/animation.hpp>
#include <config/tile.hpp>
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
