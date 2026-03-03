// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/helper/asset_manager.hpp>

#include <entt/entt.hpp>

namespace pd::systems::helper
{
	auto AssetManager::load_font(entt::registry& registry, const std::string_view path) noexcept -> id_type
	{
		auto& font_manager = registry.ctx().get<asset::FontManager>();

		return font_manager.load(path);
	}

	auto AssetManager::unload_font(entt::registry& registry, const std::string_view path) noexcept -> bool
	{
		auto& font_manager = registry.ctx().get<asset::FontManager>();

		return font_manager.erase(path);
	}

	auto AssetManager::get_font(entt::registry& registry, const std::string_view path) noexcept -> font_type
	{
		const auto& font_manager = registry.ctx().get<const asset::FontManager>();

		return font_manager.get(path);
	}

	auto AssetManager::load_texture(entt::registry& registry, const std::string_view path) noexcept -> id_type
	{
		auto& texture_manager = registry.ctx().get<asset::TextureManager>();

		return texture_manager.load(path);
	}

	auto AssetManager::unload_texture(entt::registry& registry, const std::string_view path) noexcept -> bool
	{
		auto& texture_manager = registry.ctx().get<asset::TextureManager>();

		return texture_manager.erase(path);
	}

	auto AssetManager::get_texture(entt::registry& registry, const std::string_view path) noexcept -> texture_type
	{
		const auto& texture_manager = registry.ctx().get<const asset::TextureManager>();

		return texture_manager.get(path);
	}

	auto AssetManager::load_sound(entt::registry& registry, const std::string_view path) noexcept -> id_type
	{
		auto& sound_manager = registry.ctx().get<asset::SoundManager>();

		return sound_manager.load(path);
	}

	auto AssetManager::unload_sound(entt::registry& registry, const std::string_view path) noexcept -> bool
	{
		auto& sound_manager = registry.ctx().get<asset::SoundManager>();

		return sound_manager.erase(path);
	}

	auto AssetManager::get_sound(entt::registry& registry, const std::string_view path) noexcept -> sound_type
	{
		const auto& sound_manager = registry.ctx().get<const asset::SoundManager>();

		return sound_manager.get(path);
	}

	auto AssetManager::load_music(entt::registry& registry, const std::string_view path) noexcept -> id_type
	{
		auto& music_manager = registry.ctx().get<asset::MusicManager>();

		return music_manager.load(path);
	}

	auto AssetManager::unload_music(entt::registry& registry, const std::string_view path) noexcept -> bool
	{
		auto& music_manager = registry.ctx().get<asset::MusicManager>();

		return music_manager.erase(path);
	}

	auto AssetManager::get_music(entt::registry& registry, const std::string_view path) noexcept -> music_type
	{
		const auto& music_manager = registry.ctx().get<const asset::MusicManager>();

		return music_manager.get(path);
	}
}
