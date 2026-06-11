// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/entity/fwd.hpp>

namespace pd::accessor
{
	namespace property_detail
	{
		class Value final
		{
		public:
			float value;

			constexpr explicit(false) Value(const float v) noexcept
				: value{v} {}

			[[nodiscard]] constexpr auto valid() const noexcept -> bool
			{
				return value >= 0;
			}

			constexpr explicit(false) operator float() const noexcept
			{
				return value;
			}
		};
	}

	class Property final
	{
	public:
		using value_type = property_detail::Value;

		constexpr static value_type invalid_value{-1};

		// 获取一个实体的生命值,如果目标实体没有Actor组件则返回invalid_value
		[[nodiscard]] static auto get_health(entt::registry& registry, entt::entity entity) noexcept -> value_type;

		// 获取一个实体的最大生命值,如果目标实体没有Actor组件则返回invalid_value
		[[nodiscard]] static auto get_max_health(entt::registry& registry, entt::entity entity) noexcept -> value_type;

		// 设置一个实体的生命值,如果目标实体没有Actor组件则什么也不做
		static auto set_health(entt::registry& registry, entt::entity entity, float new_health) noexcept -> void;

		// 设置一个实体的最大生命值,如果目标实体没有Actor组件则什么也不做
		static auto set_max_health(entt::registry& registry, entt::entity entity, float new_max_health) noexcept -> void;

		// 获取一个实体的魔法值,如果目标实体没有Actor组件则返回invalid_value
		[[nodiscard]] static auto get_mana(entt::registry& registry, entt::entity entity) noexcept -> value_type;

		// 获取一个实体的最大魔法值,如果目标实体没有Actor组件则返回invalid_value
		[[nodiscard]] static auto get_max_mana(entt::registry& registry, entt::entity entity) noexcept -> value_type;

		// 设置一个实体的魔法值,如果目标实体没有Actor组件则什么也不做
		static auto set_mana(entt::registry& registry, entt::entity entity, float new_mana) noexcept -> void;

		// 设置一个实体的最大魔法值,如果目标实体没有Actor组件则什么也不做
		static auto set_max_mana(entt::registry& registry, entt::entity entity, float new_max_mana) noexcept -> void;
	};
}
