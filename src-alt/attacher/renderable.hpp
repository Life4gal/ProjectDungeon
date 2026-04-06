// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <components/renderable.hpp>

#include <entt/fwd.hpp>

namespace pd::attacher
{
	class Renderable final
	{
	public:
		using render_layer = components::renderable::RenderLayer;
		using asset_id = manager::AssetId;
		using rect_type = components::renderable::Rect;
		using origin_type = components::renderable::Origin;

		// 为一个实体附加渲染组件,如果目标实体已经存在渲染组件则会进行覆盖
		static auto attach(
			entt::registry& registry,
			entt::entity entity,
			render_layer render_layer,
			manager::AssetId texture_id,
			rect_type rect,
			origin_type origin,
			sf::Color color = sf::Color::White
		) noexcept -> void;

		// 为一个实体移除渲染组件,如果目标实体没有渲染组件则什么也不做
		static auto deattach(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 获取一个实体的渲染层,如果目标实体没有渲染组件则返回RenderLayer{0}
		[[nodiscard]] static auto get_render_layer(entt::registry& registry, entt::entity entity) noexcept -> render_layer;

		// 设置一个实体的渲染层,如果目标实体没有渲染组件则什么也不做
		static auto set_render_layer(entt::registry& registry, entt::entity entity, render_layer new_render_layer) noexcept -> void;

		// 获取一个实体的纹理ID,如果目标实体没有渲染组件则返回InvalidAssetId
		[[nodiscard]] static auto get_texture_id(entt::registry& registry, entt::entity entity) noexcept -> asset_id;

		// 设置一个实体的纹理ID,如果目标实体没有渲染组件则什么也不做
		static auto set_texture_id(entt::registry& registry, entt::entity entity, asset_id new_texture_id) noexcept -> void;

		// 获取一个实体的矩形,如果目标实体没有渲染组件则返回{0,0,0,0}
		[[nodiscard]] static auto get_rect(entt::registry& registry, entt::entity entity) noexcept -> rect_type;

		// 设置一个实体的矩形,如果目标实体没有渲染组件则什么也不做
		static auto set_rect(entt::registry& registry, entt::entity entity, rect_type new_rect) noexcept -> void;

		// 获取一个实体的原点,如果目标实体没有渲染组件则返回{0,0}
		[[nodiscard]] static auto get_origin(entt::registry& registry, entt::entity entity) noexcept -> origin_type;

		// 设置一个实体的原点,如果目标实体没有渲染组件则什么也不做
		static auto set_origin(entt::registry& registry, entt::entity entity, origin_type new_origin) noexcept -> void;

		// 获取一个实体的颜色,如果目标实体没有渲染组件则返回白色
		[[nodiscard]] static auto get_color(entt::registry& registry, entt::entity entity) noexcept -> sf::Color;

		// 设置一个实体的颜色,如果目标实体没有渲染组件则什么也不做
		static auto set_color(entt::registry& registry, entt::entity entity, sf::Color new_color) noexcept -> void;

		// 隐藏一个实体,如果目标实体没有渲染组件则什么也不做
		static auto hide(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 显示一个实体,如果目标实体没有渲染组件则什么也不做
		static auto show(entt::registry& registry, entt::entity entity) noexcept -> void;
	};
}
