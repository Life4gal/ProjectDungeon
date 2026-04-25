// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/floor.hpp>

#include <manager/resource.hpp>

#include <component/floor.hpp>

#include <entt/entt.hpp>

namespace pd::factory
{
	using namespace component;

	auto Floor::spawn(entt::registry& registry, const blueprint::Floor& floor) noexcept -> entt::entity
	{
		const auto entity = registry.create();

		// transform
		{
			const auto& transform = floor.transform;

			registry.emplace<transform::Position>(entity, sf::Vector2f{transform.x, transform.y});
			registry.emplace<transform::Scale>(entity, sf::Vector2f{transform.scale_x, transform.scale_y});
			registry.emplace<transform::Rotation>(entity, sf::degrees(transform.rotation));
		}

		// sprite
		{
			const auto& sprite = floor.sprite;

			registry.emplace<sprite::Texture>(entity, manager::Texture::load(std::string_view{sprite.texture}));
			registry.emplace<sprite::Position>(entity, sf::Vector2i{sprite.x, sprite.y});
			registry.emplace<sprite::Size>(entity, sf::Vector2i{sprite.width, sprite.height});
			registry.emplace<sprite::Origin>(entity, sf::Vector2i{sprite.origin_x, sprite.origin_y});
			registry.emplace<sprite::Color>(entity, sf::Color::White);
			registry.emplace<sprite::Scale>(entity, sf::Vector2f{1, 1});
		}

		registry.emplace<floor::Floor>(entity);

		return entity;
	}

	auto Floor::destroy(entt::registry& registry, const entt::entity entity) noexcept -> void
	{
		registry.destroy(entity);
	}

	auto Floor::destroy_all(entt::registry& registry) noexcept -> void
	{
		const auto view = registry.view<floor::Floor>();
		registry.destroy(view.begin(), view.end());
	}
}
