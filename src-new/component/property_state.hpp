// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <entt/entity/fwd.hpp>

#include <SFML/System/Vector2.hpp>

namespace pd::component::property_state
{
	// 生命值条与实体位置的偏移
	class HealthBarOffset final
	{
	public:
		sf::Vector2f offset;
	};

	// 生命值条的大小
	class HealthBarSize final
	{
	public:
		sf::Vector2f size;
	};

	// 魔法值条与实体位置的偏移
	class ManaBarOffset final
	{
	public:
		sf::Vector2f offset;
	};

	// 魔法值条的大小
	class ManaBarSize final
	{
	public:
		sf::Vector2f size;
	};

	// 受伤记录
	class DamageHistory
	{
	public:
		class Info final
		{
		public:
			entt::entity attacker;
			float damage;
		};

		std::vector<Info> damage_history;
	};
}
