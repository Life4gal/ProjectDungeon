// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/fwd.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Angle.hpp>

namespace pd::systems::helper
{
	class Transform final
	{
	public:
		// 为一个实体附加变换组件
		// 如果目标实体已经存在变换组件则会进行覆盖
		static auto attach(
			entt::registry& registry,
			entt::entity entity,
			sf::Vector2f position,
			sf::Vector2f scale = sf::Vector2f{1, 1},
			sf::Angle rotation = sf::degrees(0)
		) noexcept -> void;

		// 为一个实体移除变换组件
		static auto deattach(entt::registry& registry, entt::entity entity_with_transform) noexcept -> void;

		[[nodiscard]] static auto get_position(entt::registry& registry, entt::entity entity_with_transform) noexcept -> sf::Vector2f;

		static auto set_position(entt::registry& registry, entt::entity entity_with_transform, sf::Vector2f position) noexcept -> void;

		[[nodiscard]] static auto get_scale(entt::registry& registry, entt::entity entity_with_transform) noexcept -> sf::Vector2f;

		static auto set_scale(entt::registry& registry, entt::entity entity_with_transform, sf::Vector2f scale) noexcept -> void;

		[[nodiscard]] static auto get_rotation(entt::registry& registry, entt::entity entity_with_transform) noexcept -> sf::Angle;

		static auto set_rotation(entt::registry& registry, entt::entity entity_with_transform, sf::Angle rotation) noexcept -> void;
	};
}
