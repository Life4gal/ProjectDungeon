// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <functional>

namespace pd
{
	class Game;

	namespace components
	{
		// ==========================================
		// 上下文组件
		// ==========================================

		// 游戏实例的引用
		class Game final : public std::reference_wrapper<pd::Game> {};

		// ==========================================
		// 实体组件
		// ==========================================

		//
	}
}
