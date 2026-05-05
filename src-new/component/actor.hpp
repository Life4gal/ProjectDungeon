// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <entt/entity/fwd.hpp>

#include <SFML/System/Vector2.hpp>

namespace pd::component::actor
{
	// 当前生命值
	class Health final
	{
	public:
		float health;
	};

	// 最大生命值
	class HealthMax final
	{
	public:
		float health;
	};

	// 当前魔法值
	class Mana final
	{
	public:
		float mana;
	};

	// 最大魔法值
	class ManaMax final
	{
	public:
		float mana;
	};

	// 最大速度
	// 当前速度(Velocity: sf::Vector2f)需要基于物理体获得
	// FIXME: 如果Actor没有物理体呢?
	class Speed final
	{
	public:
		float speed;
	};

	// 生命值条的大小
	class HealthBarSize final
	{
	public:
		sf::Vector2f size;
	};

	// 生命值条与Actor位置的偏移
	class HealthBarOffset final
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

	// 魔法值条与Actor位置的偏移
	class ManaBarOffset final
	{
	public:
		sf::Vector2f offset;
	};

	// ============================================

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
