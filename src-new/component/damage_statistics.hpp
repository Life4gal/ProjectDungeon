// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <entt/entity/fwd.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

namespace pd::component::damage_statistics
{
	// 攻击记录
	class AttackRecords final
	{
	public:
		class Record final
		{
		public:
			// 受害者
			// 受害者死亡前有效
			entt::entity victim;

			// 造成伤害的时间
			sf::Time time;

			// 造成伤害时攻击者位置
			sf::Vector2f attacker_position;
			// 造成伤害时受害者位置
			sf::Vector2f victim_position;

			// 造成伤害前受害者的生命值
			float victim_health;
			// 造成的伤害
			float damage;
		};

		std::vector<Record> records;
	};

	// 受伤记录
	class InjuryRecords final
	{
	public:
		class Record final
		{
		public:
			// 攻击者实体
			entt::entity attacker;
			// 这是攻击者的第几次攻击(AttackRecords::records的下标)
			std::size_t attack_index;
		};

		std::vector<Record> records;
	};
}
