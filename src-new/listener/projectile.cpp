// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <listener/projectile.hpp>

#include <manager/event.hpp>

#include <event/actor.hpp>

#include <component/projectile.hpp>
#include <component/state.hpp>

#include <prometheus/meta/enumeration.hpp>
#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <spdlog/spdlog.h>

namespace pd::listener::projectile
{
	using namespace prometheus;

	auto on_contact_begin(entt::registry& registry, const event::physics::ContactBegin& contact_begin) noexcept -> void
	{
		namespace tags = component::tags;
		namespace state = component::state;
		namespace projectile = component::projectile;

		using manager::Event;

		// 不是飞弹直接跳过
		if (contact_begin.a_type != blueprint::PhysicsShapeType::PROJECTILE and contact_begin.b_type != blueprint::PhysicsShapeType::PROJECTILE)
		{
			return;
		}
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Projectile>(contact_begin.a) or registry.all_of<tags::Projectile>(contact_begin.b));

		const auto a = contact_begin.a_type == blueprint::PhysicsShapeType::PROJECTILE;

		const auto projectile_entity = a ? contact_begin.a : contact_begin.b;
		const auto other_entity = a ? contact_begin.b : contact_begin.a;
		const auto other_type = a ? contact_begin.b_type : contact_begin.a_type;

		SPDLOG_INFO(
			"ContactBegin: [PROJECTILE]=0x{:08X}, [OTHER]=0x{:08X}({})",
			entt::to_integral(projectile_entity),
			entt::to_integral(other_entity),
			meta::name_of(other_type)
		);

		// 如果命中了敌人或者玩家
		// 发出伤害事件
		if (other_type == blueprint::PhysicsShapeType::PLAYER or other_type == blueprint::PhysicsShapeType::ENEMY)
		{
			const auto [owner] = registry.get<const projectile::Owner>(projectile_entity);
			const auto [damage] = registry.get<const projectile::Damage>(projectile_entity);

			Event::enqueue(event::actor::Hurt{.attacker = owner, .victim = other_entity, .damage = damage});
		}

		// TODO: 何时销毁飞弹?
		// 如果在ContactBegin时销毁实体则无论如何都不会触发ContactEnd(因为实体已经无效)
		// registry.destroy(projectile_entity);
		// 这里应该使用emplace_or_replace,而不是emplace,因为飞弹可能同时接触到多个物理体,也就是一帧内处理多个接触事件
		// TODO: 但是这也意味着一发飞弹可以同时命中多个敌人,造成多次伤害,这是我们预期的行为吗?
		registry.emplace_or_replace<state::Dead>(projectile_entity);
	}

	auto on_contact_end(entt::registry& registry, const event::physics::ContactEnd& contact_end) noexcept -> void
	{
		namespace tags = component::tags;

		// 不是飞弹直接跳过
		if (contact_end.a_type != blueprint::PhysicsShapeType::PROJECTILE and contact_end.b_type != blueprint::PhysicsShapeType::PROJECTILE)
		{
			return;
		}
		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<tags::Projectile>(contact_end.a) or registry.all_of<tags::Projectile>(contact_end.b));

		const auto a = contact_end.a_type == blueprint::PhysicsShapeType::PROJECTILE;

		const auto projectile_entity = a ? contact_end.a : contact_end.b;
		const auto other_entity = a ? contact_end.b : contact_end.a;
		const auto other_type = a ? contact_end.b_type : contact_end.a_type;

		SPDLOG_INFO(
			"ContactEnd: [PROJECTILE]=0x{:08X}, [OTHER]=0x{:08X}({})",
			entt::to_integral(projectile_entity),
			entt::to_integral(other_entity),
			meta::name_of(other_type)
		);
	}
}
