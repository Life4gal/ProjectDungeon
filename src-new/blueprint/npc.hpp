// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <blueprint/detail/character.hpp>
#include <blueprint/detail/ai.hpp>

namespace pd::blueprint
{
	// NPC
	class Npc final
	{
	public:
		// NPC类型
		NpcType type;

		// NPC角色
		Character character;

		// NPC AI
		Ai ai;

		//
	};
}
