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

	//

	// ==========================================
	// 上下文组件
	// ==========================================

	//

	// ==========================================
	// 实体组件
	// ==========================================

	enum class Type : std::uint8_t
	{
		// 钱币
		COIN,
		// 物品
		ITEM,
		// 装备
		EQUIPMENT,
		// 食物/药水
		CONSUMABLE,

		// 爆炸陷阱
		EXPLOSION_TRAP,
		// 落石陷阱
		ROCKFALL_TRAP,
	};

	// 动画计时器
	// 仅在播放动画时存在
	class Timer final
	{
	public:
		sf::Time timer;
	};
}
