// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/render.hpp>
#include <blueprint/detail/collision.hpp>
#include <blueprint/detail/property.hpp>
#include <blueprint/detail/ai.hpp>
#include <blueprint/detail/name.hpp>

namespace pd::blueprint
{
	// NPC
	class Npc final
	{
	public:
		// NPC的初始位置
		Position position;

		// 渲染(必须是动态精灵吗?)
		Sprite::Dynamic sprite;

		// 碰撞体(一定有碰撞体吗?)
		Collision collision;

		// NPC类型
		NpcType type;
		// NPC属性
		Property property;
		// NPC AI
		Ai ai;
		// NPC名称
		Name name;

		//
	};
}
