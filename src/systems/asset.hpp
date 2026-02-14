// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <systems/system.hpp>

#include <entt/resource/resource.hpp>

namespace sf
{
	class Font;
	class Texture;
	class SoundBuffer;
	class Music;
}

namespace pd::systems
{
	class Asset final : public System<Asset>
	{
		friend System;

		static auto do_loaded(entt::registry& registry) noexcept -> void;

		static auto do_initialized(entt::registry& registry) noexcept -> void;

		static auto do_unloaded(entt::registry& registry) noexcept -> void;

		static auto do_update(entt::registry& registry, sf::Time delta) noexcept -> void;

		static auto do_render(entt::registry& registry, sf::RenderWindow& window) noexcept -> void;

	public:
		// ========================================
		// 定义一套static接口,便于直接读取registry.ctx
		// ========================================

		[[nodiscard]] static auto font(entt::registry& registry, entt::id_type id) noexcept -> entt::resource<const sf::Font>;

		[[nodiscard]] static auto texture(entt::registry& registry, entt::id_type id) noexcept -> entt::resource<const sf::Texture>;

		[[nodiscard]] static auto sound(entt::registry& registry, entt::id_type id) noexcept -> entt::resource<const sf::SoundBuffer>;

		[[nodiscard]] static auto music(entt::registry& registry, entt::id_type id) noexcept -> entt::resource<const sf::Music>;
	};
}
