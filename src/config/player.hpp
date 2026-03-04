// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <config/config_reader.hpp>
#include <config/set.hpp>

namespace pd::config
{
	class Player final
	{
	public:
		// 动画ID
		std::string animation_id;

		// 生命值
		float health;
		// 魔法值
		float mana;
		// 移动速度
		float speed;

		// 武器的名称列表
		std::vector<std::string> weapons;
	};

	// 玩家集
	// 玩家ID -> 玩家配置
	// JSON:
	// { "players": "/path/to/players.json" }
	// { "players": { "player-id-1": { Player }, "player-id-2": { Player }, ... } }
	class PlayerSet final : public Set<Player>
	{
	public:
		using Set::Set;
	};

	[[nodiscard]] auto load_player_from_json(Player& player, const ConfigReader::json_format& json) noexcept -> bool;
	[[nodiscard]] auto load_player_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<Player>;

	[[nodiscard]] auto load_player_set_from_json(PlayerSet& player_set, const ConfigReader::json_format& json) noexcept -> bool;
	[[nodiscard]] auto load_player_set_from_json(const ConfigReader::json_format& json) noexcept -> std::optional<PlayerSet>;
}
