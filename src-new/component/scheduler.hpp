// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

namespace pd::component::scheduler
{
	// =============
	// 调度器单独实体
	// =============

	// 调度器所有者
	class Owner final
	{
	public:
		entt::entity owner;
	};

	// 当条件满足时会立刻销毁调度器

	// =============
	// 调度器附加在所有者上
	// =============

	// 当条件满足时所有者会附件该组件(而不是立刻销毁)
	class Finished final {};

	// ===================================
	// 时间
	// ===================================

	// 总持续时间
	class Duration final
	{
	public:
		sf::Time duration;
	};

	// 已持续时间
	class Elapsed final
	{
	public:
		sf::Time elapsed;
	};

	// ===================================
	// 位置
	// ===================================

	// 距离的平方
	class DistanceSquared final
	{
	public:
		float value;
	};

	// 锚定位置
	class AnchorPosition final
	{
	public:
		sf::Vector2f position;
	};

	// ===================================
	//
	// ===================================
}
