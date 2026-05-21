// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

namespace pd::helper
{
	class Actor final
	{
	public:
		constexpr static auto invalid_value = -1.0f;

		// 获取一个实体的生命值,如果目标实体没有Actor组件则返回invalid_value
		[[nodiscard]] static auto get_health(entt::registry& registry, entt::entity entity) noexcept -> float;

		// 获取一个实体的最大生命值,如果目标实体没有Actor组件则返回invalid_value
		[[nodiscard]] static auto get_max_health(entt::registry& registry, entt::entity entity) noexcept -> float;

		// 设置一个实体的生命值,如果目标实体没有Actor组件则什么也不做
		static auto set_health(entt::registry& registry, entt::entity entity, float new_health) noexcept -> void;

		// 设置一个实体的最大生命值,如果目标实体没有Actor组件则什么也不做
		static auto set_max_health(entt::registry& registry, entt::entity entity, float new_max_health) noexcept -> void;

		// 获取一个实体的魔法值,如果目标实体没有Actor组件则返回invalid_value
		[[nodiscard]] static auto get_mana(entt::registry& registry, entt::entity entity) noexcept -> float;

		// 获取一个实体的最大魔法值,如果目标实体没有Actor组件则返回invalid_value
		[[nodiscard]] static auto get_max_mana(entt::registry& registry, entt::entity entity) noexcept -> float;

		// 设置一个实体的魔法值,如果目标实体没有Actor组件则什么也不做
		static auto set_mana(entt::registry& registry, entt::entity entity, float new_mana) noexcept -> void;

		// 设置一个实体的最大魔法值,如果目标实体没有Actor组件则什么也不做
		static auto set_max_mana(entt::registry& registry, entt::entity entity, float new_max_mana) noexcept -> void;

		// =====================================

		// 对指定实体造成伤害,如果目标实体没有Actor组件则什么也不做
		//
		// Event:
		//  event::actor::Damaged
		static auto hurt(entt::registry& registry, entt::entity victim, entt::entity attacker, float damage) noexcept -> void;

		// 杀死指定实体,如果目标实体没有Actor组件则什么也不做
		//
		// Event:
		//  event::actor::Slayed
		static auto kill(entt::registry& registry, entt::entity victim, entt::entity attacker) noexcept -> void;
	};
}
