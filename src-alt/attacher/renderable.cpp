// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <attacher/renderable.hpp>

#include <entt/entt.hpp>

namespace pd::attacher
{
	using namespace components;

	auto Renderable::attach(
		entt::registry& registry,
		const entt::entity entity,
		const render_layer render_layer,
		const manager::AssetId texture_id,
		const rect_type rect,
		const origin_type origin,
		const sf::Color color
	) noexcept -> void
	{
		registry.emplace_or_replace<renderable::RenderLayer>(entity, render_layer);
		registry.emplace_or_replace<renderable::Texture>(entity, texture_id);
		registry.emplace_or_replace<renderable::Rect>(entity, rect);
		registry.emplace_or_replace<renderable::Origin>(entity, origin);
		registry.emplace_or_replace<renderable::Color>(entity, color);
	}

	auto Renderable::deattach(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.remove<renderable::RenderLayer>(entity);
		registry.remove<renderable::Texture>(entity);
		registry.remove<renderable::Rect>(entity);
		registry.remove<renderable::Origin>(entity);
		registry.remove<renderable::Color>(entity);

		registry.remove<renderable::Invisible>(entity);
	}

	auto Renderable::get_render_layer(entt::registry& registry, const entt::entity entity) noexcept -> render_layer
	{
		const auto* render_layer = registry.try_get<const renderable::RenderLayer>(entity);
		if (render_layer == nullptr)
		{
			return renderable::RenderLayer{0};
		}

		return *render_layer;
	}

	auto Renderable::set_render_layer(entt::registry& registry, const entt::entity entity, const render_layer new_render_layer) noexcept -> void
	{
		auto* render_layer = registry.try_get<renderable::RenderLayer>(entity);
		if (render_layer == nullptr)
		{
			return;
		}

		*render_layer = new_render_layer;
	}

	auto Renderable::get_texture_id(entt::registry& registry, const entt::entity entity) noexcept -> asset_id
	{
		const auto* texture_id = registry.try_get<const renderable::Texture>(entity);
		if (texture_id == nullptr)
		{
			return manager::InvalidAssetId;
		}

		return texture_id->id;
	}

	auto Renderable::set_texture_id(entt::registry& registry, const entt::entity entity, const asset_id new_texture_id) noexcept -> void
	{
		auto* texture_id = registry.try_get<renderable::Texture>(entity);
		if (texture_id == nullptr)
		{
			return;
		}

		texture_id->id = new_texture_id;
	}

	auto Renderable::get_rect(entt::registry& registry, const entt::entity entity) noexcept -> rect_type
	{
		const auto* rect = registry.try_get<const renderable::Rect>(entity);
		if (rect == nullptr)
		{
			return {};
		}

		return *rect;
	}

	auto Renderable::set_rect(entt::registry& registry, const entt::entity entity, const rect_type new_rect) noexcept -> void
	{
		auto* rect = registry.try_get<renderable::Rect>(entity);
		if (rect == nullptr)
		{
			return;
		}

		*rect = new_rect;
	}

	auto Renderable::get_origin(entt::registry& registry, const entt::entity entity) noexcept -> origin_type
	{
		const auto* origin = registry.try_get<const renderable::Origin>(entity);
		if (origin == nullptr)
		{
			return {};
		}

		return *origin;
	}

	auto Renderable::set_origin(entt::registry& registry, const entt::entity entity, const origin_type new_origin) noexcept -> void
	{
		auto* origin = registry.try_get<renderable::Origin>(entity);
		if (origin == nullptr)
		{
			return;
		}

		*origin = new_origin;
	}

	auto Renderable::get_color(entt::registry& registry, const entt::entity entity) noexcept -> sf::Color
	{
		const auto* color = registry.try_get<const renderable::Color>(entity);
		if (color == nullptr)
		{
			return sf::Color::White;
		}

		return color->color;
	}

	auto Renderable::set_color(entt::registry& registry, const entt::entity entity, const sf::Color new_color) noexcept -> void
	{
		auto* color = registry.try_get<renderable::Color>(entity);
		if (color == nullptr)
		{
			return;
		}

		color->color = new_color;
	}

	auto Renderable::hide(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.emplace_or_replace<renderable::Invisible>(entity);
	}

	auto Renderable::show(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.remove<renderable::Invisible>(entity);
	}
}
