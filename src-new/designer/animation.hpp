// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/animation.hpp>

namespace pd::designer
{
	class Animation final
	{
	public:
		// ==============================================
		// 敌人
		// ==============================================

		[[nodiscard]] static auto rat() noexcept -> blueprint::Animation;

		[[nodiscard]] static auto slime() noexcept -> blueprint::Animation;

		[[nodiscard]] static auto bat() noexcept -> blueprint::Animation;

		// ==============================================
		// NPC
		// ==============================================

		// ==============================================
		// 玩家
		// ==============================================

		[[nodiscard]] static auto player_default() noexcept -> blueprint::Animation;
	};
}
