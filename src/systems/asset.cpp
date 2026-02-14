// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/asset.hpp>

#include <game/names.hpp>

#include <asset/font_loader.hpp>
#include <asset/texture_loader.hpp>
#include <asset/sound_loader.hpp>
#include <asset/music_loader.hpp>

#include <entt/entt.hpp>

namespace pd::systems
{
	auto Asset::do_loaded(entt::registry& registry) noexcept -> void
	{
		// 创建loader
		auto& font_loader = registry.ctx().emplace<asset::FontLoader>();
		auto& texture_loader = registry.ctx().emplace<asset::TextureLoader>();
		auto& sound_loader = registry.ctx().emplace<asset::SoundLoader>();
		auto& music_loader = registry.ctx().emplace<asset::MusicLoader>();

		// 获取资源
		// 这些实体由Blueprint::initialize创建
		auto& font_map = registry.ctx().get<const asset::map_type>(Names::asset_font);
		auto& texture_map = registry.ctx().get<const asset::map_type>(Names::asset_texture);
		auto& sound_map = registry.ctx().get<const asset::map_type>(Names::asset_sound);
		auto& music_map = registry.ctx().get<const asset::map_type>(Names::asset_music);

		// 载入资源
		font_loader.load(font_map);
		texture_loader.load(texture_map);
		sound_loader.load(sound_map);
		music_loader.load(music_map);
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
		const auto& font_loader = registry.ctx().emplace<const asset::FontLoader>();

		return font_loader.get(id);
	}

	auto Asset::texture(entt::registry& registry, const entt::id_type id) noexcept -> entt::resource<const sf::Texture>
	{
		const auto& texture_loader = registry.ctx().emplace<const asset::TextureLoader>();

		return texture_loader.get(id);
	}

	auto Asset::sound(entt::registry& registry, const entt::id_type id) noexcept -> entt::resource<const sf::SoundBuffer>
	{
		const auto& sound_loader = registry.ctx().emplace<const asset::SoundLoader>();

		return sound_loader.get(id);
	}

	auto Asset::music(entt::registry& registry, const entt::id_type id) noexcept -> entt::resource<const sf::Music>
	{
		const auto& music_loader = registry.ctx().emplace<const asset::MusicLoader>();

		return music_loader.get(id);
	}
}
