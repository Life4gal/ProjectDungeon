// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/enemy.hpp>

#include <manager/event.hpp>

#include <event/actor.hpp>

#include <component/enemy.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

namespace pd::listener::enemy
{
	auto on_contact_begin(entt::registry& registry, const event::physics::ContactBegin& contact_begin) noexcept -> void
	{
		namespace tags = component::tags;
		namespace enemy = component::enemy;

		using manager::Event;

		// 不是敌人直接跳过
		if (contact_begin.a_type != blueprint::PhysicsShapeType::ENEMY and contact_begin.b_type != blueprint::PhysicsShapeType::ENEMY)
		{
			return;
		}
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Enemy>(contact_begin.a) or registry.all_of<tags::Enemy>(contact_begin.b));

		const auto a = contact_begin.a_type == blueprint::PhysicsShapeType::ENEMY;

		const auto enemy_entity = a ? contact_begin.a : contact_begin.b;
		const auto other_entity = a ? contact_begin.b : contact_begin.a;
		const auto other_type = a ? contact_begin.b_type : contact_begin.a_type;

		// 不是敌人接触玩家直接跳过
		if (other_type != blueprint::PhysicsShapeType::PLAYER)
		{
			return;
		}

		SPDLOG_INFO(
			"ContactBegin: [ENEMY]=0x{:08X}, [PLAYER]=0x{:08X}",
			entt::to_integral(enemy_entity),
			entt::to_integral(other_entity)
		);

		const auto [contact_damage] = registry.get<const enemy::ContactDamage>(enemy_entity);

		Event::enqueue(event::actor::Hurt{.attacker = enemy_entity, .victim = other_entity, .damage = contact_damage});
	}

	auto on_contact_end(entt::registry& registry, const event::physics::ContactEnd& contact_end) noexcept -> void
	{
		namespace tags = component::tags;

		// 不是敌人直接跳过
		if (contact_end.a_type != blueprint::PhysicsShapeType::ENEMY and contact_end.b_type != blueprint::PhysicsShapeType::ENEMY)
		{
			return;
		}
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Enemy>(contact_end.a) or registry.all_of<tags::Enemy>(contact_end.b));

		const auto a = contact_end.a_type == blueprint::PhysicsShapeType::ENEMY;

		const auto enemy_entity = a ? contact_end.a : contact_end.b;
		const auto other_entity = a ? contact_end.b : contact_end.a;
		const auto other_type = a ? contact_end.b_type : contact_end.a_type;

		// 不是敌人接触玩家直接跳过
		if (other_type != blueprint::PhysicsShapeType::PLAYER)
		{
			return;
		}

		SPDLOG_INFO(
			"ContactEnd: [ENEMY]=0x{:08X}, [PLAYER]=0x{:08X}",
			entt::to_integral(enemy_entity),
			entt::to_integral(other_entity)
		);
	}
}
