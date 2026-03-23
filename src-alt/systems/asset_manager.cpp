// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <systems/asset_manager.hpp>

#include <algorithm>
#include <ranges>

#include <components/asset_manager.hpp>

#include <systems/game_config.hpp>

#include <prometheus/platform/os.hpp>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

namespace pd::systems
{
	auto AssetManager::load_font(entt::registry& registry, const std::string_view path) noexcept -> id_type
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::FontManager>());

		auto& font_manager = registry.ctx().get<components::FontManager>();
		return font_manager.load(path);
	}

	auto AssetManager::get_font(entt::registry& registry, const id_type id) noexcept -> font_type
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::FontManager>());

		const auto& font_manager = registry.ctx().get<const components::FontManager>();
		return font_manager.get(id);
	}

	auto AssetManager::get_font(entt::registry& registry, const std::string_view path) noexcept -> font_type
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::FontManager>());

		const auto& font_manager = registry.ctx().get<const components::FontManager>();
		return font_manager.get(path);
	}

	auto AssetManager::unload_font(entt::registry& registry, const id_type id) noexcept -> bool
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::FontManager>());

		auto& font_manager = registry.ctx().get<components::FontManager>();
		return font_manager.erase(id);
	}

	auto AssetManager::unload_font(entt::registry& registry, const std::string_view path) noexcept -> bool
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::FontManager>());

		auto& font_manager = registry.ctx().get<components::FontManager>();
		return font_manager.erase(path);
	}

	auto AssetManager::unload_all_font(entt::registry& registry) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::FontManager>());

		auto& font_manager = registry.ctx().get<components::FontManager>();
		font_manager.clear();
	}

	auto AssetManager::load_texture(entt::registry& registry, const std::string_view path) noexcept -> id_type
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::TextureManager>());

		auto& texture_manager = registry.ctx().get<components::TextureManager>();
		return texture_manager.load(path);
	}

	auto AssetManager::get_texture(entt::registry& registry, const id_type id) noexcept -> texture_type
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::TextureManager>());

		const auto& texture_manager = registry.ctx().get<const components::TextureManager>();
		return texture_manager.get(id);
	}

	auto AssetManager::get_texture(entt::registry& registry, const std::string_view path) noexcept -> texture_type
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::TextureManager>());

		const auto& texture_manager = registry.ctx().get<const components::TextureManager>();
		return texture_manager.get(path);
	}

	auto AssetManager::unload_texture(entt::registry& registry, const id_type id) noexcept -> bool
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::TextureManager>());

		auto& texture_manager = registry.ctx().get<components::TextureManager>();
		return texture_manager.erase(id);
	}

	auto AssetManager::unload_texture(entt::registry& registry, const std::string_view path) noexcept -> bool
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::TextureManager>());

		auto& texture_manager = registry.ctx().get<components::TextureManager>();
		return texture_manager.erase(path);
	}

	auto AssetManager::unload_all_texture(entt::registry& registry) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::TextureManager>());

		auto& texture_manager = registry.ctx().get<components::TextureManager>();
		texture_manager.clear();
	}

	auto AssetManager::load_sound(entt::registry& registry, const std::string_view path) noexcept -> id_type
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::SoundManager>());

		auto& sound_manager = registry.ctx().get<components::SoundManager>();
		return sound_manager.load(path);
	}

	auto AssetManager::get_sound(entt::registry& registry, const id_type id) noexcept -> sound_type
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::SoundManager>());

		const auto& sound_manager = registry.ctx().get<const components::SoundManager>();
		return sound_manager.get(id);
	}

	auto AssetManager::get_sound(entt::registry& registry, const std::string_view path) noexcept -> sound_type
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::SoundManager>());

		const auto& sound_manager = registry.ctx().get<const components::SoundManager>();
		return sound_manager.get(path);
	}

	auto AssetManager::unload_sound(entt::registry& registry, const id_type id) noexcept -> bool
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::SoundManager>());

		auto& sound_manager = registry.ctx().get<components::SoundManager>();
		return sound_manager.erase(id);
	}

	auto AssetManager::unload_sound(entt::registry& registry, const std::string_view path) noexcept -> bool
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::SoundManager>());

		auto& sound_manager = registry.ctx().get<components::SoundManager>();
		return sound_manager.erase(path);
	}

	auto AssetManager::unload_all_sound(entt::registry& registry) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::SoundManager>());

		auto& sound_manager = registry.ctx().get<components::SoundManager>();
		sound_manager.clear();
	}

	namespace
	{
		auto do_play_sound(entt::registry& registry, const AssetManager::sound_type& sound, const bool unique) noexcept -> bool
		{
			PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::SoundChannels>());

			auto& [channels] = registry.ctx().get<components::SoundChannels>();

			if (unique)
			{
				// 如果存在还在播放的相同声音则跳过
				const auto it = std::ranges::find_if(
					channels,
					[&sound](const auto& channel) noexcept -> bool
					{
						return channel.getStatus() != sf::Sound::Status::Stopped and std::addressof(channel.getBuffer()) == sound.operator->();
					}
				);
				if (it != channels.end())
				{
					return false;
				}
			}

			// 寻找一个空闲的声音通道来播放声音
			const auto it = std::ranges::find(channels, sf::Sound::Status::Stopped, &sf::Sound::getStatus);
			// 如果找不到空闲声音通道且当前数量已经超过最大值则跳过
			if (it == channels.end())
			{
				if (const auto max_sound_channels = GameConfig::get_max_sound_channels(registry);
					channels.size() > max_sound_channels)
				{
					SPDLOG_WARN("All channels busy, drop the sound...");
					return false;
				}
			}

			const auto volume = GameConfig::get_sound_volume(registry);

			auto& channel = *it;
			channel.setBuffer(*sound);
			channel.setVolume(volume);
			channel.play();

			return true;
		}
	}

	auto AssetManager::play_sound(entt::registry& registry, const id_type id) noexcept -> bool
	{
		const auto sound = get_sound(registry, id);
		if (not sound)
		{
			return false;
		}

		return do_play_sound(registry, sound, false);
	}

	auto AssetManager::play_sound(entt::registry& registry, const std::string_view path) noexcept -> bool
	{
		const auto sound = get_sound(registry, path);
		if (not sound)
		{
			return false;
		}

		return do_play_sound(registry, sound, false);
	}

	auto AssetManager::play_sound_unique(entt::registry& registry, const id_type id) noexcept -> bool
	{
		const auto sound = get_sound(registry, id);
		return do_play_sound(registry, sound, true);
	}

	auto AssetManager::play_sound_unique(entt::registry& registry, const std::string_view path) noexcept -> bool
	{
		const auto sound = get_sound(registry, path);
		return do_play_sound(registry, sound, true);
	}

	auto AssetManager::load_music(entt::registry& registry, const std::string_view path) noexcept -> id_type
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::MusicManager>());

		auto& music_manager = registry.ctx().get<components::MusicManager>();
		return music_manager.load(path);
	}

	auto AssetManager::get_music(entt::registry& registry, const id_type id) noexcept -> music_type
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::MusicManager>());

		auto& music_manager = registry.ctx().get<components::MusicManager>();
		return music_manager.get(id);
	}

	auto AssetManager::get_music(entt::registry& registry, const std::string_view path) noexcept -> music_type
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::MusicManager>());

		auto& music_manager = registry.ctx().get<components::MusicManager>();
		return music_manager.get(path);
	}

	auto AssetManager::unload_music(entt::registry& registry, const id_type id) noexcept -> bool
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::MusicManager>());

		auto& music_manager = registry.ctx().get<components::MusicManager>();
		return music_manager.erase(id);
	}

	auto AssetManager::unload_music(entt::registry& registry, const std::string_view path) noexcept -> bool
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::MusicManager>());

		auto& music_manager = registry.ctx().get<components::MusicManager>();
		return music_manager.erase(path);
	}

	auto AssetManager::unload_all_music(entt::registry& registry) noexcept -> void
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::MusicManager>());

		auto& music_manager = registry.ctx().get<components::MusicManager>();
		music_manager.clear();
	}

	namespace
	{
		auto do_play_music(entt::registry& registry, const AssetManager::music_type& music, const bool restart, const bool loop) noexcept -> bool
		{
			PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::MusicChannel>());

			auto& [channel] = registry.ctx().get<components::MusicChannel>();

			// 如果当前正在播放音乐且是同一首音乐且不需要重启则跳过
			if (channel != nullptr and channel == music.operator->() and not restart)
			{
				return true;
			}

			if (channel != nullptr)
			{
				// 停止正在播放的音乐
				channel->stop();
			}
			channel = music.operator->();

			const auto volume = GameConfig::get_music_volume(registry);

			channel->setLooping(loop);
			channel->setVolume(volume);
			music->play();

			return true;
		}
	}

	auto AssetManager::play_music(entt::registry& registry, const id_type id, const bool loop) noexcept -> bool
	{
		const auto music = get_music(registry, id);
		if (not music)
		{
			return false;
		}

		return do_play_music(registry, music, false, loop);
	}

	auto AssetManager::play_music(entt::registry& registry, const std::string_view path, const bool loop) noexcept -> bool
	{
		const auto music = get_music(registry, path);
		if (not music)
		{
			return false;
		}

		return do_play_music(registry, music, false, loop);
	}

	auto AssetManager::play_music_or_restart(entt::registry& registry, const id_type id, const bool loop) noexcept -> bool
	{
		const auto music = get_music(registry, id);
		if (not music)
		{
			return false;
		}

		return do_play_music(registry, music, true, loop);
	}

	auto AssetManager::play_music_or_restart(entt::registry& registry, const std::string_view path, const bool loop) noexcept -> bool
	{
		const auto music = get_music(registry, path);
		if (not music)
		{
			return false;
		}

		return do_play_music(registry, music, true, loop);
	}

	namespace
	{
		auto do_stop_music(entt::registry& registry, const AssetManager::music_type& music) noexcept -> bool
		{
			if (music->getStatus() != sf::Music::Status::Playing)
			{
				return false;
			}

			music->stop();
			registry.ctx().insert_or_assign<components::MusicChannel>(components::MusicChannel{.music = nullptr});

			return true;
		}
	}

	auto AssetManager::stop_music(entt::registry& registry, const id_type id) noexcept -> bool
	{
		const auto music = get_music(registry, id);
		if (not music)
		{
			return false;
		}

		return do_stop_music(registry, music);
	}

	auto AssetManager::stop_music(entt::registry& registry, const std::string_view path) noexcept -> bool
	{
		const auto music = get_music(registry, path);
		if (not music)
		{
			return false;
		}

		return do_stop_music(registry, music);
	}

	auto AssetManager::stop_music(entt::registry& registry) noexcept -> bool
	{
		PROMETHEUS_PLATFORM_ASSUME(registry.ctx().contains<components::MusicChannel>());

		auto& [channel] = registry.ctx().get<components::MusicChannel>();

		if (channel == nullptr)
		{
			return false;
		}

		channel->stop();
		return true;
	}
}
