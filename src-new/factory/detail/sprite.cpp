// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <factory/detail/sprite.hpp>

#include <manager/resource.hpp>

#include <blueprint/detail/sprite.hpp>

#include <component/sprite.hpp>

#include <entt/entt.hpp>

namespace pd::factory::detail
{
	auto attach(entt::registry& registry, const entt::entity entity, const blueprint::Sprite& sprite) noexcept -> void
	{
		using namespace component;

		// ====================
		// 纹理
		// ====================

		auto texture = manager::Texture::load(std::filesystem::path{sprite.texture});

		registry.emplace<sprite::Texture>(entity, std::move(texture));
		registry.emplace<sprite::TexturePosition>(entity, sf::Vector2f{sprite.position.x, sprite.position.y});
		registry.emplace<sprite::TextureSize>(entity, sf::Vector2f{sprite.size.width, sprite.size.height});
		registry.emplace<sprite::TextureOrigin>(entity, sf::Vector2f{sprite.origin.x, sprite.origin.y});

		// ====================
		// 渲染
		// ====================

		registry.emplace<sprite::RenderPositionOffset>(entity, sf::Vector2f{0, 0});
		registry.emplace<sprite::RenderScale>(entity, sf::Vector2f{sprite.scale.x, sprite.scale.y});
		registry.emplace<sprite::RenderRotation>(entity, sf::degrees(0));
		registry.emplace<sprite::RenderColor>(entity, sf::Color::White);
	}
}
