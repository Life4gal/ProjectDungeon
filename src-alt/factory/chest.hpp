// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

namespace pd::factory
{
	class Chest final
	{
	public:
		// 创建宝箱
		// todo: 参数?
		static auto spawn(entt::registry& registry) noexcept -> entt::entity;

		// 销毁宝箱
		static auto destroy(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 销毁所有宝箱
		static auto destroy_all(entt::registry& registry) noexcept -> void;
	};
}
