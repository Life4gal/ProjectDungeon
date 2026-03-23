// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string_view>

#include <manager/asset_manager_type.hpp>

#include <entt/fwd.hpp>

namespace pd::systems
{
	class AssetManager final
	{
	public:
		using id_type = manager::AssetId;

		using font_type = manager::font_type;
		using texture_type = manager::texture_type;
		using sound_type = manager::sound_type;
		using music_type = manager::music_type;

		// =========================================================
		// FONT
		// =========================================================

		static auto load_font(entt::registry& registry, std::string_view path) noexcept -> id_type;

		[[nodiscard]] static auto get_font(entt::registry& registry, id_type id) noexcept -> font_type;

		[[nodiscard]] static auto get_font(entt::registry& registry, std::string_view path) noexcept -> font_type;

		static auto unload_font(entt::registry& registry, id_type id) noexcept -> bool;

		static auto unload_font(entt::registry& registry, std::string_view path) noexcept -> bool;

		static auto unload_all_font(entt::registry& registry) noexcept -> void;

		// =========================================================
		// TEXTURE
		// =========================================================

		static auto load_texture(entt::registry& registry, std::string_view path) noexcept -> id_type;

		[[nodiscard]] static auto get_texture(entt::registry& registry, id_type id) noexcept -> texture_type;

		[[nodiscard]] static auto get_texture(entt::registry& registry, std::string_view path) noexcept -> texture_type;

		static auto unload_texture(entt::registry& registry, id_type id) noexcept -> bool;

		static auto unload_texture(entt::registry& registry, std::string_view path) noexcept -> bool;

		static auto unload_all_texture(entt::registry& registry) noexcept -> void;

		// =========================================================
		// SOUND
		// =========================================================

		static auto load_sound(entt::registry& registry, std::string_view path) noexcept -> id_type;

		[[nodiscard]] static auto get_sound(entt::registry& registry, id_type id) noexcept -> sound_type;

		[[nodiscard]] static auto get_sound(entt::registry& registry, std::string_view path) noexcept -> sound_type;

		static auto unload_sound(entt::registry& registry, id_type id) noexcept -> bool;

		static auto unload_sound(entt::registry& registry, std::string_view path) noexcept -> bool;

		static auto unload_all_sound(entt::registry& registry) noexcept -> void;

		// ===============
		// FIXME: 把这些接口移动到合适的位置

		// 播放一个音效
		// false:
		//  -指定的音效未加载
		//  -指定的音效正在播放
		//  -不存在空闲音效通道
		static auto play_sound(entt::registry& registry, id_type id) noexcept -> bool;

		// 播放一个音效
		// false:
		//  -指定的音效未加载
		//  -指定的音效正在播放
		//  -不存在空闲音效通道
		static auto play_sound(entt::registry& registry, std::string_view path) noexcept -> bool;

		// 播放一个音效,如果指定音效正在播放则跳过
		// false:
		//  -指定的音效未加载
		//  -不存在空闲音效通道
		static auto play_sound_unique(entt::registry& registry, id_type id) noexcept -> bool;

		// 播放一个音效,如果指定音效正在播放则跳过
		// false:
		//  -指定的音效未加载
		//  -不存在空闲音效通道
		static auto play_sound_unique(entt::registry& registry, std::string_view path) noexcept -> bool;

		// =========================================================
		// MUSIC
		// =========================================================

		static auto load_music(entt::registry& registry, std::string_view path) noexcept -> id_type;

		[[nodiscard]] static auto get_music(entt::registry& registry, id_type id) noexcept -> music_type;

		[[nodiscard]] static auto get_music(entt::registry& registry, std::string_view path) noexcept -> music_type;

		static auto unload_music(entt::registry& registry, id_type id) noexcept -> bool;

		static auto unload_music(entt::registry& registry, std::string_view path) noexcept -> bool;

		static auto unload_all_music(entt::registry& registry) noexcept -> void;

		// ===============
		// FIXME: 把这些接口移动到合适的位置

		// 播放一个音乐,如果音乐正在播放则跳过
		// false:
		//  -指定的音乐未加载
		//  -指定的音乐正在播放
		static auto play_music(entt::registry& registry, id_type id, bool loop = true) noexcept -> bool;

		// 播放一个音乐,如果音乐正在播放则跳过
		// false:
		//  -指定的音乐未加载
		//  -指定的音乐正在播放
		static auto play_music(entt::registry& registry, std::string_view path, bool loop = true) noexcept -> bool;

		// 播放一个音乐,如果音乐正在播放则重启
		// false:
		//  -指定的音乐未加载
		static auto play_music_or_restart(entt::registry& registry, id_type id, bool loop = true) noexcept -> bool;

		// 播放一个音乐,如果音乐正在播放则重启
		// false:
		//  -指定的音乐未加载
		static auto play_music_or_restart(entt::registry& registry, std::string_view path, bool loop = true) noexcept -> bool;

		// 停止一个音乐
		// false:
		//  -指定的音乐未加载
		//  -指定的音乐未播放
		static auto stop_music(entt::registry& registry, id_type id) noexcept -> bool;

		// 停止一个音乐
		// false:
		//  -指定的音乐未加载
		//  -指定的音乐未播放
		static auto stop_music(entt::registry& registry, std::string_view path) noexcept -> bool;

		// 停止当前音乐
		// false:
		//  -未指定音乐播放
		static auto stop_music(entt::registry& registry) noexcept -> bool;
	};
}
