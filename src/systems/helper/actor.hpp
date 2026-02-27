// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

namespace pd::systems::helper
{
	class Actor final
	{
	public:
		// 为一个实体附加actor组件
		// 如果目标实体已经存在actor组件则会进行覆盖
		static auto attach(
			entt::registry& registry,
			entt::entity entity,
			float health,
			float mana,
			float speed
		) noexcept -> void;

		// 为一个实体移除actor组件
		static auto deattach(entt::registry& registry, entt::entity entity_with_actor) noexcept -> void;

		[[nodiscard]] static auto get_health(entt::registry& registry, entt::entity entity_with_actor) noexcept -> float;

		static auto set_health(entt::registry& registry, entt::entity entity_with_actor, float health) noexcept -> void;

		[[nodiscard]] static auto get_health_max(entt::registry& registry, entt::entity entity_with_actor) noexcept -> float;

		static auto set_health_max(entt::registry& registry, entt::entity entity_with_actor, float health_max) noexcept -> void;

		[[nodiscard]] static auto get_mana(entt::registry& registry, entt::entity entity_with_actor) noexcept -> float;

		static auto set_mana(entt::registry& registry, entt::entity entity_with_actor, float mana) noexcept -> void;

		[[nodiscard]] static auto get_mana_max(entt::registry& registry, entt::entity entity_with_actor) noexcept -> float;

		static auto set_mana_max(entt::registry& registry, entt::entity entity_with_actor, float mana_max) noexcept -> void;

		[[nodiscard]] static auto get_speed(entt::registry& registry, entt::entity entity_with_actor) noexcept -> float;

		[[nodiscard]] static auto get_speed_squared(entt::registry& registry, entt::entity entity_with_actor) noexcept -> float;

		static auto set_speed(entt::registry& registry, entt::entity entity_with_actor, float speed) noexcept -> void;
	};
}
