// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <manager/resource.hpp>

#include <blueprint/detail/sprite.hpp>

#include <component/sprite.hpp>

#include <entt/entity/registry.hpp>

namespace pd::factory::detail
{
	inline auto attach(entt::registry& registry, const entt::entity entity, const blueprint::Sprite& sprite) noexcept -> void
	{
		using namespace component;

		registry.emplace<sprite::Texture>(entity, manager::Texture::load(std::string_view{sprite.texture}));
		registry.emplace<sprite::Position>(entity, sf::Vector2i{sprite.x, sprite.y});
		registry.emplace<sprite::Size>(entity, sf::Vector2i{sprite.width, sprite.height});
		registry.emplace<sprite::Origin>(entity, sf::Vector2i{sprite.origin_x, sprite.origin_y});
		registry.emplace<sprite::Color>(entity, sf::Color::White);
		registry.emplace<sprite::Scale>(entity, sf::Vector2f{1, 1});
	}
}
