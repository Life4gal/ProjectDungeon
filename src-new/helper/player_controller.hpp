// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

namespace pd::helper
{
	class PlayerController final
	{
	public:
		// ==========================================
		// 控制实体移动
		// ==========================================

		// TODO: 编写一套可用的输入管理器

		static auto left(entt::registry& registry) noexcept -> void;
		static auto right(entt::registry& registry) noexcept -> void;
		static auto stop_horizontal(entt::registry& registry) noexcept -> void;
		static auto up(entt::registry& registry) noexcept -> void;
		static auto down(entt::registry& registry) noexcept -> void;
		static auto stop_vertical(entt::registry& registry) noexcept -> void;
	};
}
