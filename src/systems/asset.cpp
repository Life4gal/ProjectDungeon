// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/asset.hpp>

#include <asset/manager.hpp>

#include <entt/entt.hpp>

namespace pd::systems
{
	auto Asset::do_loaded(entt::registry& registry) noexcept -> void
	{
		// 创建manager
		auto& font_manager = registry.ctx().emplace<asset::FontManager>();
		auto& texture_manager = registry.ctx().emplace<asset::TextureManager>();
		auto& sound_manager = registry.ctx().emplace<asset::SoundManager>();
		auto& music_manager = registry.ctx().emplace<asset::MusicManager>();

		// 资源路径映射表
		// 这些实体由Blueprint::initialize创建
		auto& font_map = registry.ctx().get<const asset::FontMap>();
		auto& texture_map = registry.ctx().get<const asset::TextureMap>();
		auto& sound_map = registry.ctx().get<const asset::SoundMap>();
		auto& music_map = registry.ctx().get<const asset::MusicMap>();

		// 载入资源
		font_manager.load(font_map);
		texture_manager.load(texture_map);
		sound_manager.load(sound_map);
		music_manager.load(music_map);
	}

	auto Asset::do_initialized([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
	}

	auto Asset::do_unloaded([[maybe_unused]] entt::registry& registry) noexcept -> void
	{
		// 什么也不做
	}

	auto Asset::do_update([[maybe_unused]] entt::registry& registry, [[maybe_unused]] const sf::Time delta) noexcept -> void
	{
		// 什么也不做
	}

	auto Asset::do_render([[maybe_unused]] entt::registry& registry, [[maybe_unused]] sf::RenderWindow& window) noexcept -> void
	{
		// 什么也不做
	}

	auto Asset::font(entt::registry& registry, const entt::id_type id) noexcept -> entt::resource<const sf::Font>
	{
		const auto& font_manager = registry.ctx().emplace<const asset::FontManager>();

		return font_manager.get(id);
	}

	auto Asset::texture(entt::registry& registry, const entt::id_type id) noexcept -> entt::resource<const sf::Texture>
	{
		const auto& texture_manager = registry.ctx().emplace<const asset::TextureManager>();

		return texture_manager.get(id);
	}

	auto Asset::sound(entt::registry& registry, const entt::id_type id) noexcept -> entt::resource<const sf::SoundBuffer>
	{
		const auto& sound_manager = registry.ctx().emplace<const asset::SoundManager>();

		return sound_manager.get(id);
	}

	auto Asset::music(entt::registry& registry, const entt::id_type id) noexcept -> entt::resource<const sf::Music>
	{
		const auto& music_manager = registry.ctx().emplace<const asset::MusicManager>();

		return music_manager.get(id);
	}
}
