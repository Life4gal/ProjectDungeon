// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/character.hpp>

namespace pd::designer
{
	class Character final
	{
	public:
		// ==============================================
		// 敌人
		// ==============================================

		[[nodiscard]] static auto rat() noexcept -> blueprint::Character;

		[[nodiscard]] static auto slime() noexcept -> blueprint::Character;

		[[nodiscard]] static auto bat() noexcept -> blueprint::Character;

		// ==============================================
		// NPC
		// ==============================================

		// ==============================================
		// 玩家
		// ==============================================

		[[nodiscard]] static auto player_default() noexcept -> blueprint::Character;
	};
}
