// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <config/dungeon.hpp>

#include <entt/entity/fwd.hpp>

namespace pd::config
{
	class Dungeon;
}

namespace pd::components::dungeon
{
	// ==========================================
	// 上下文组件
	// ==========================================

	// 与level不同,dungeon直接保存config::Dungeon对象
	class Dungeon final
	{
	public:
		config::Dungeon dungeon;
	};

	// 当前关卡实体
	// 我们不需要保存所有关卡的实体,因为只能逐个闯关,且不能回头
	class Level final
	{
	public:
		entt::entity entity;
	};

	// ==========================================
	// 实体组件
	// ==========================================

	//
}
