// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <span>

#include <entt/entity/fwd.hpp>

namespace pd::events::chest
{
	// 禁用目标宝箱实体
	class Disable final
	{
	public:
		entt::entity chest;
	};

	// 启用目标宝箱实体
	class Enable final
	{
	public:
		entt::entity chest;
	};

	// 禁用所有目标宝箱实体
	class DisableAll final
	{
	public:
		std::span<const entt::entity> chests;
	};

	// 启用所有目标宝箱实体
	class EnableAll final
	{
	public:
		std::span<const entt::entity> chests;
	};

	// 任意实体与宝箱实体接触
	class Contacted final
	{
	public:
		entt::entity chest;
		entt::entity entity;
	};
}
