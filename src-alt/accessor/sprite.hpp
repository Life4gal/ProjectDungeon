// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <manager/asset_fwd.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace pd::accessor
{
	class Sprite final
	{
	public:
		// ===================================================

		// 初始化系统
		static auto initialize_system(entt::registry& registry) noexcept -> void;

		// 销毁系统
		static auto destroy_system(entt::registry& registry) noexcept -> void;

		// ===================================================

		// 为一个实体附加渲染组件,如果目标实体已经存在渲染组件则什么也不做
		static auto attach(
			entt::registry& registry,
			entt::entity entity,
			manager::AssetId texture_id,
			sf::Vector2i rect_position,
			sf::Vector2i rect_size,
			sf::Vector2i rect_origin,
			sf::Color color = sf::Color::White,
			sf::Vector2f scale = {1.f, 1.f}
		) noexcept -> void;

		// 为一个实体附加渲染组件,如果目标实体已经存在渲染组件则会进行覆盖
		static auto attach_force(
			entt::registry& registry,
			entt::entity entity,
			manager::AssetId texture_id,
			sf::Vector2i rect_position,
			sf::Vector2i rect_size,
			sf::Vector2i rect_origin,
			sf::Color color = sf::Color::White,
			sf::Vector2f scale = {1.f, 1.f}
		) noexcept -> void;

		// 为一个实体移除渲染组件,如果目标实体没有渲染组件则什么也不做
		static auto deattach(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 获取一个实体的纹理ID,如果目标实体没有渲染组件则返回InvalidAssetId
		[[nodiscard]] static auto get_texture_id(entt::registry& registry, entt::entity entity) noexcept -> manager::AssetId;

		// 设置一个实体的纹理ID,如果目标实体没有渲染组件则什么也不做
		static auto set_texture_id(entt::registry& registry, entt::entity entity, manager::AssetId new_texture_id) noexcept -> void;

		// 获取一个实体的纹理位置(在纹理中的位置),如果目标实体没有渲染组件则返回{0,0}
		[[nodiscard]] static auto get_position(entt::registry& registry, entt::entity entity) noexcept -> sf::Vector2i;

		// 设置一个实体的纹理位置(在纹理中的位置),如果目标实体没有渲染组件则什么也不做
		static auto set_position(entt::registry& registry, entt::entity entity, sf::Vector2i new_position) noexcept -> void;

		// 获取一个实体的纹理大小,如果目标实体没有渲染组件则返回{0,0}
		[[nodiscard]] static auto get_size(entt::registry& registry, entt::entity entity) noexcept -> sf::Vector2i;

		// 设置一个实体的纹理大小,如果目标实体没有渲染组件则什么也不做
		static auto set_size(entt::registry& registry, entt::entity entity, sf::Vector2i new_size) noexcept -> void;

		// 获取一个实体的纹理原点,如果目标实体没有渲染组件则返回{0,0}
		[[nodiscard]] static auto get_origin(entt::registry& registry, entt::entity entity) noexcept -> sf::Vector2i;

		// 设置一个实体的纹理原点,如果目标实体没有渲染组件则什么也不做
		static auto set_origin(entt::registry& registry, entt::entity entity, sf::Vector2i new_origin) noexcept -> void;

		// 获取一个实体的颜色,如果目标实体没有渲染组件则返回白色
		[[nodiscard]] static auto get_color(entt::registry& registry, entt::entity entity) noexcept -> sf::Color;

		// 设置一个实体的颜色,如果目标实体没有渲染组件则什么也不做
		static auto set_color(entt::registry& registry, entt::entity entity, sf::Color new_color) noexcept -> void;

		// 获取一个实体的缩放,如果目标实体没有渲染组件则返回{1,1}
		[[nodiscard]] static auto get_scale(entt::registry& registry, entt::entity entity) noexcept -> sf::Vector2f;

		// 设置一个实体的缩放,如果目标实体没有渲染组件则什么也不做
		static auto set_scale(entt::registry& registry, entt::entity entity, sf::Vector2f new_scale) noexcept -> void;

		// 隐藏一个实体,如果目标实体没有渲染组件则什么也不做
		static auto hide(entt::registry& registry, entt::entity entity) noexcept -> void;

		// 显示一个实体,如果目标实体没有渲染组件则什么也不做
		static auto show(entt::registry& registry, entt::entity entity) noexcept -> void;
	};
}
