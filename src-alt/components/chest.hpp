// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <SFML/System/Time.hpp>

namespace pd::components::chest
{
	// ==========================================
	// 非实体组件
	// ==========================================

	enum class Type : std::uint8_t
	{
		// 标准宝箱
		STANDARD,
		// 出口房间宝箱
		EXIT,
	};

	enum class TrapType : std::uint8_t
	{
		NONE,

		// 爆炸陷阱
		EXPLOSION,
		// 落石陷阱
		ROCKFALL,
	};

	// ==========================================
	// 上下文组件
	// ==========================================

	//

	// ==========================================
	// 实体组件
	// ==========================================

	// 宝箱状态
	enum class State : std::uint8_t
	{
		CLOSED,
		OPENED,
	};

	// 陷阱动画计时器
	// 仅在播放动画时存在
	class Timer final
	{
	public:
		sf::Time timer;
	};
}
