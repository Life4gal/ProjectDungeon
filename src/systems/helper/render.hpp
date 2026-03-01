// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <config/render_layer.hpp>

#include <entt/fwd.hpp>

#include <SFML/Graphics/Color.hpp>

namespace pd::config
{
	class AnimationFrame;
}

namespace pd::systems::helper
{
	class Render final
	{
	public:
		// 为一个实体附加渲染组件
		// 如果目标实体已经存在渲染组件则会进行覆盖
		static auto attach(
			entt::registry& registry,
			entt::entity entity,
			const config::AnimationFrame& animation_frame,
			config::RenderLayer render_layer,
			sf::Color color = sf::Color::White
		) noexcept -> void;

		// 为一个实体移除渲染组件
		static auto deattach(entt::registry& registry, entt::entity entity_with_render) noexcept -> void;

		// 使指定实体不可见
		static auto hide(entt::registry& registry, entt::entity entity_with_render) noexcept -> void;

		// 使指定实体可见
		static auto show(entt::registry& registry, entt::entity entity_with_render) noexcept -> void;

		// 获取指定实体的动画帧
		[[nodiscard]] static auto get_animation_frame(const entt::registry& registry, entt::entity entity_with_render) noexcept -> const config::AnimationFrame&;

		// 获取指定实体的渲染层级
		[[nodiscard]] static auto get_layer(const entt::registry& registry, entt::entity entity_with_render) noexcept -> config::RenderLayer;

		// 改变指定实体的渲染层级
		static auto change_layer(entt::registry& registry, entt::entity entity_with_render, config::RenderLayer new_layer) noexcept -> void;

		// 获取指定实体的颜色
		[[nodiscard]] static auto get_color(const entt::registry& registry, entt::entity entity_with_render) noexcept -> sf::Color;

		// 改变指定实体的颜色
		static auto change_color(entt::registry& registry, entt::entity entity_with_render, sf::Color new_color) noexcept -> void;
	};
}
