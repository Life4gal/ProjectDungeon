// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/actor.hpp>

#include <components/actor.hpp>

#include <prometheus/platform/os.hpp>
#include <entt/entt.hpp>

namespace pd::systems::helper
{
	auto Actor::attach(
		entt::registry& registry,
		const entt::entity entity,
		const float health,
		const float mana,
		const float speed
	) noexcept -> void
	{
		using namespace components;

		registry.emplace_or_replace<actor::Health>(entity, actor::Health{.current = health, .max = health});
		registry.emplace_or_replace<actor::Mana>(entity, actor::Mana{.current = mana, .max = mana});
		registry.emplace_or_replace<actor::Speed>(entity, actor::Speed{.speed = speed, .speed_squared = speed * speed});
	}

	auto Actor::deattach(entt::registry& registry, const entt::entity entity_with_actor) noexcept -> void
	{
		using namespace components;

		registry.remove<actor::Health>(entity_with_actor);
		registry.remove<actor::Mana>(entity_with_actor);
		registry.remove<actor::Speed>(entity_with_actor);
	}

	auto Actor::get_health(entt::registry& registry, const entt::entity entity_with_actor) noexcept -> float
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<actor::Health>(entity_with_actor));

		return registry.get<const actor::Health>(entity_with_actor).current;
	}

	auto Actor::set_health(entt::registry& registry, const entt::entity entity_with_actor, const float health) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<actor::Health>(entity_with_actor));

		const auto h = registry.get<const actor::Health>(entity_with_actor);
		registry.replace<actor::Health>(entity_with_actor, actor::Health{.current = health, .max = h.max});
	}

	auto Actor::get_health_max(entt::registry& registry, const entt::entity entity_with_actor) noexcept -> float
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<actor::Health>(entity_with_actor));

		return registry.get<const actor::Health>(entity_with_actor).max;
	}

	auto Actor::set_health_max(entt::registry& registry, const entt::entity entity_with_actor, const float health_max) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<actor::Health>(entity_with_actor));

		const auto h = registry.get<const actor::Health>(entity_with_actor);
		registry.replace<actor::Health>(entity_with_actor, actor::Health{.current = h.current, .max = health_max});
	}

	auto Actor::get_mana(entt::registry& registry, const entt::entity entity_with_actor) noexcept -> float
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<actor::Mana>(entity_with_actor));

		return registry.get<const actor::Mana>(entity_with_actor).current;
	}

	auto Actor::set_mana(entt::registry& registry, const entt::entity entity_with_actor, const float mana) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<actor::Mana>(entity_with_actor));

		const auto m = registry.get<const actor::Mana>(entity_with_actor);
		registry.replace<actor::Mana>(entity_with_actor, actor::Mana{.current = mana, .max = m.max});
	}

	auto Actor::get_mana_max(entt::registry& registry, const entt::entity entity_with_actor) noexcept -> float
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<actor::Mana>(entity_with_actor));

		return registry.get<const actor::Mana>(entity_with_actor).max;
	}

	auto Actor::set_mana_max(entt::registry& registry, const entt::entity entity_with_actor, const float mana_max) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<actor::Mana>(entity_with_actor));

		const auto m = registry.get<const actor::Mana>(entity_with_actor);
		registry.replace<actor::Mana>(entity_with_actor, actor::Mana{.current = m.current, .max = mana_max});
	}

	auto Actor::get_speed(entt::registry& registry, const entt::entity entity_with_actor) noexcept -> float
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<actor::Speed>(entity_with_actor));

		return registry.get<const actor::Speed>(entity_with_actor).speed;
	}

	auto Actor::get_speed_squared(entt::registry& registry, const entt::entity entity_with_actor) noexcept -> float
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<actor::Speed>(entity_with_actor));

		return registry.get<const actor::Speed>(entity_with_actor).speed_squared;
	}

	auto Actor::set_speed(entt::registry& registry, const entt::entity entity_with_actor, const float speed) noexcept -> void
	{
		using namespace components;

		PROMETHEUS_PLATFORM_ASSUME(registry.all_of<actor::Speed>(entity_with_actor));

		const auto s = registry.get<const actor::Speed>(entity_with_actor);
		registry.replace<actor::Speed>(entity_with_actor, actor::Speed{.speed = speed, .speed_squared = speed * speed});
	}
}
