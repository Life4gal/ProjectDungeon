// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/render.hpp>
#include <blueprint/detail/collision.hpp>
#include <blueprint/detail/property.hpp>
#include <blueprint/detail/ai.hpp>

namespace pd::blueprint
{
	// NPC 类型
	enum class NpcType : std::uint8_t
	{
		// 商人
		MERCHANT,
	};

	// NPC
	class Npc final
	{
	public:
		// 商人的初始位置
		Position position;

		// 渲染(必须是动态精灵吗?)
		DynamicSprite sprite;

		// 碰撞体(一定有碰撞体吗?)
		Collision collision;

		// NPC类型
		NpcType type;
		// NPC属性
		Property property;
		// 商人AI
		Ai ai;

		//
	};
}
