// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <accessor/sprite.hpp>

#include <components/sprite.hpp>

#include <entt/entt.hpp>

namespace pd::accessor
{
	using namespace components;

	auto Sprite::initialize_system([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		//
	}

	auto Sprite::destroy_system([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		//
	}

	auto Sprite::attach(
		entt::registry& registry,
		const entt::entity entity,
		const manager::AssetId texture_id,
		const sf::Vector2i rect_position,
		const sf::Vector2i rect_size,
		const sf::Vector2i rect_origin,
		const sf::Color color,
		const sf::Vector2f scale
	) noexcept -> void
	{
		if (registry.any_of<sprite::Texture, sprite::Position, sprite::Size, sprite::Origin, sprite::Color, sprite::Color>(entity))
		{
			return;
		}

		registry.emplace<sprite::Texture>(entity, texture_id);
		registry.emplace<sprite::Position>(entity, rect_position);
		registry.emplace<sprite::Size>(entity, rect_size);
		registry.emplace<sprite::Origin>(entity, rect_origin);
		registry.emplace<sprite::Color>(entity, color);
		registry.emplace<sprite::Scale>(entity, scale);
	}

	auto Sprite::attach_force(
		entt::registry& registry,
		const entt::entity entity,
		const manager::AssetId texture_id,
		const sf::Vector2i rect_position,
		const sf::Vector2i rect_size,
		const sf::Vector2i rect_origin,
		const sf::Color color,
		const sf::Vector2f scale
	) noexcept -> void
	{
		registry.emplace_or_replace<sprite::Texture>(entity, texture_id);
		registry.emplace_or_replace<sprite::Position>(entity, rect_position);
		registry.emplace_or_replace<sprite::Size>(entity, rect_size);
		registry.emplace_or_replace<sprite::Origin>(entity, rect_origin);
		registry.emplace_or_replace<sprite::Color>(entity, color);
		registry.emplace_or_replace<sprite::Scale>(entity, scale);
	}

	auto Sprite::deattach(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.remove<sprite::Texture>(entity);
		registry.remove<sprite::Position>(entity);
		registry.remove<sprite::Size>(entity);
		registry.remove<sprite::Origin>(entity);
		registry.remove<sprite::Color>(entity);
		registry.remove<sprite::Scale>(entity);

		registry.remove<sprite::Invisible>(entity);
	}

	auto Sprite::get_texture_id(entt::registry& registry, const entt::entity entity) noexcept -> manager::AssetId
	{
		const auto* texture_id = registry.try_get<const sprite::Texture>(entity);
		if (texture_id == nullptr)
		{
			return manager::InvalidAssetId;
		}

		return texture_id->id;
	}

	auto Sprite::set_texture_id(entt::registry& registry, const entt::entity entity, const manager::AssetId new_texture_id) noexcept -> void
	{
		auto* texture_id = registry.try_get<sprite::Texture>(entity);
		if (texture_id == nullptr)
		{
			return;
		}

		texture_id->id = new_texture_id;
	}

	auto Sprite::get_position(entt::registry& registry, const entt::entity entity) noexcept -> sf::Vector2i
	{
		const auto* position = registry.try_get<const sprite::Position>(entity);
		if (position == nullptr)
		{
			return {0, 0};
		}

		return position->position;
	}

	auto Sprite::set_position(entt::registry& registry, const entt::entity entity, const sf::Vector2i new_position) noexcept -> void
	{
		auto* position = registry.try_get<sprite::Position>(entity);
		if (position == nullptr)
		{
			return;
		}

		position->position = new_position;
	}

	auto Sprite::get_size(entt::registry& registry, const entt::entity entity) noexcept -> sf::Vector2i
	{
		const auto* size = registry.try_get<const sprite::Size>(entity);
		if (size == nullptr)
		{
			return {0, 0};
		}

		return size->size;
	}

	auto Sprite::set_size(entt::registry& registry, const entt::entity entity, const sf::Vector2i new_size) noexcept -> void
	{
		auto* size = registry.try_get<sprite::Size>(entity);
		if (size == nullptr)
		{
			return;
		}

		size->size = new_size;
	}

	auto Sprite::get_origin(entt::registry& registry, const entt::entity entity) noexcept -> sf::Vector2i
	{
		const auto* origin = registry.try_get<const sprite::Origin>(entity);
		if (origin == nullptr)
		{
			return {0, 0};
		}

		return origin->origin;
	}

	auto Sprite::set_origin(entt::registry& registry, const entt::entity entity, const sf::Vector2i new_origin) noexcept -> void
	{
		auto* origin = registry.try_get<sprite::Origin>(entity);
		if (origin == nullptr)
		{
			return;
		}

		origin->origin = new_origin;
	}

	auto Sprite::get_color(entt::registry& registry, const entt::entity entity) noexcept -> sf::Color
	{
		const auto* color = registry.try_get<const sprite::Color>(entity);
		if (color == nullptr)
		{
			return sf::Color::White;
		}

		return color->color;
	}

	auto Sprite::set_color(entt::registry& registry, const entt::entity entity, const sf::Color new_color) noexcept -> void
	{
		auto* color = registry.try_get<sprite::Color>(entity);
		if (color == nullptr)
		{
			return;
		}

		color->color = new_color;
	}

	auto Sprite::get_scale(entt::registry& registry, const entt::entity entity) noexcept -> sf::Vector2f
	{
		const auto* scale = registry.try_get<const sprite::Scale>(entity);
		if (scale == nullptr)
		{
			return {1, 1};
		}

		return scale->scale;
	}

	auto Sprite::set_scale(entt::registry& registry, const entt::entity entity, const sf::Vector2f new_scale) noexcept -> void
	{
		auto* scale = registry.try_get<sprite::Scale>(entity);
		if (scale == nullptr)
		{
			return;
		}

		scale->scale = new_scale;
	}

	auto Sprite::hide(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.emplace_or_replace<sprite::Invisible>(entity);
	}

	auto Sprite::show(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.remove<sprite::Invisible>(entity);
	}
}
