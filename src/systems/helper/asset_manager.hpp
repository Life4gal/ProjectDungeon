// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <asset/font.hpp>
#include <asset/texture.hpp>
#include <asset/sound.hpp>
#include <asset/music.hpp>

#include <entt/fwd.hpp>

namespace pd::systems::helper
{
	class AssetManager final
	{
	public:
		using id_type = asset::AssetId;

		using font_type = asset::FontManager::result_type;
		using texture_type = asset::TextureManager::result_type;
		using sound_type = asset::SoundManager::result_type;
		using music_type = asset::MusicManager::result_type;

		// =========================================
		// FONT
		// =========================================

		static auto load_font(entt::registry& registry, std::string_view path) noexcept -> id_type;

		static auto unload_font(entt::registry& registry, std::string_view path) noexcept -> bool;

		[[nodiscard]] static auto get_font(entt::registry& registry, std::string_view path) noexcept -> font_type;

		// =========================================
		// TEXTURE
		// =========================================

		static auto load_texture(entt::registry& registry, std::string_view path) noexcept -> id_type;

		static auto unload_texture(entt::registry& registry, std::string_view path) noexcept -> bool;

		[[nodiscard]] static auto get_texture(entt::registry& registry, std::string_view path) noexcept -> texture_type;

		// =========================================
		// SOUND
		// =========================================

		static auto load_sound(entt::registry& registry, std::string_view path) noexcept -> id_type;

		static auto unload_sound(entt::registry& registry, std::string_view path) noexcept -> bool;

		[[nodiscard]] static auto get_sound(entt::registry& registry, std::string_view path) noexcept -> sound_type;

		// =========================================
		// MUSIC
		// =========================================

		static auto load_music(entt::registry& registry, std::string_view path) noexcept -> id_type;

		static auto unload_music(entt::registry& registry, std::string_view path) noexcept -> bool;

		[[nodiscard]] static auto get_music(entt::registry& registry, std::string_view path) noexcept -> music_type;
	};
}
