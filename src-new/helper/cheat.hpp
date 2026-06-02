// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

namespace pd::helper
{
	class Cheat final
	{
	public:
		static auto kill_enemy(entt::registry& registry, entt::entity attacker, entt::entity enemy) noexcept -> void;

		static auto kill_all_enemy(entt::registry& registry, entt::entity attacker) noexcept -> void;

		static auto kill_all_enemy(entt::registry& registry) noexcept -> void;

		static auto set_all_enemy_hp_percent(entt::registry& registry, float percent) noexcept -> void;
	};
}
