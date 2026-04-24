// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <manager/asset.hpp>

#include <var/audio.hpp>

#include <prometheus/platform/os.hpp>
#include <spdlog/spdlog.h>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

namespace pd::manager
{
	namespace detail
	{
		auto FontLoader::operator()(const std::string_view path) const noexcept -> result_type
		{
			try
			{
				const std::filesystem::path p{path};

				if (not exists(p))
				{
					SPDLOG_ERROR("载入字体[{:<20}]失败!文件不存在!", path);
					return result_type{};
				}

				auto font = std::make_shared<sf::Font>();
				if (not font->openFromFile(p))
				{
					SPDLOG_ERROR("载入字体[{:<20}]失败!", path);
					return result_type{};
				}

				// SPDLOG_INFO("载入字体[{:<20}]成功!", path);

				return font;
			}
			catch (const std::system_error& e)
			{
				SPDLOG_ERROR("载入字体[{:<20}]失败: {}", path, e.what());
				return result_type{};
			}
		}

		auto TextureLoader::operator()(const std::string_view path) const noexcept -> result_type
		{
			try
			{
				const std::filesystem::path p{path};

				if (not exists(p))
				{
					SPDLOG_ERROR("载入纹理[{:<20}]失败!文件不存在!", path);
					return result_type{};
				}

				auto texture = std::make_shared<sf::Texture>();
				if (not texture->loadFromFile(p))
				{
					SPDLOG_ERROR("载入纹理[{:<20}]失败!", path);
					return result_type{};
				}

				// SPDLOG_INFO("载入纹理[{:<20}]成功!", path);

				return texture;
			}
			catch (const std::system_error& e)
			{
				SPDLOG_ERROR("载入纹理[{:<20}]失败: {}", path, e.what());
				return result_type{};
			}
		}

		auto SoundLoader::operator()(const std::string_view path) const noexcept -> result_type
		{
			try
			{
				const std::filesystem::path p{path};

				if (not exists(p))
				{
					SPDLOG_ERROR("载入音效[{:<20}]失败!文件不存在!", path);
					return result_type{};
				}

				auto sound = std::make_shared<sf::SoundBuffer>();
				if (not sound->loadFromFile(p))
				{
					SPDLOG_ERROR("载入音效[{:<20}]失败!", path);
					return result_type{};
				}

				SPDLOG_INFO("载入音效[{:<20}]成功!", path);

				return sound;
			}
			catch (const std::system_error& e)
			{
				SPDLOG_ERROR("载入音效[{:<20}]失败: {}", path, e.what());
				return result_type{};
			}
		}

		auto MusicLoader::operator()(const std::string_view path) const noexcept -> result_type
		{
			try
			{
				const std::filesystem::path p{path};

				if (not exists(p))
				{
					SPDLOG_ERROR("载入音乐[{:<20}]失败!文件不存在!", path);
					return result_type{};
				}

				auto music = std::make_shared<sf::Music>();
				if (not music->openFromFile(p))
				{
					SPDLOG_ERROR("载入音乐[{:<20}]失败!", path);
					return result_type{};
				}

				SPDLOG_INFO("载入音乐[{:<20}]成功!", path);

				return music;
			}
			catch (const std::system_error& e)
			{
				SPDLOG_ERROR("载入音乐[{:<20}]失败: {}", path, e.what());
				return result_type{};
			}
		}
	}

	namespace
	{
		// 音效播放通道
		[[nodiscard]] auto get_sound_channels() noexcept -> std::vector<sf::Sound>&
		{
			static std::vector<sf::Sound> sound_channels{};
			return sound_channels;
		}

		auto do_play_sound(const sound_type& sound, const bool unique) noexcept -> bool
		{
			auto& channels = get_sound_channels();

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
				if (channels.size() > var::max_sound_channels)
				{
					SPDLOG_WARN("All channels busy, drop the sound...");
					return false;
				}
			}

			auto& channel = *it;
			channel.setBuffer(*sound);
			channel.setVolume(var::sound_volume);
			channel.play();

			return true;
		}
	}

	auto Sound::play(const AssetId id) noexcept -> bool
	{
		const auto sound = get(id);
		if (not sound)
		{
			return false;
		}

		return do_play_sound(sound, false);
	}

	auto Sound::play(const std::string_view path) noexcept -> bool
	{
		const auto sound = get(path);
		if (not sound)
		{
			return false;
		}

		return do_play_sound(sound, false);
	}

	auto Sound::play_unique(const AssetId id) noexcept -> bool
	{
		const auto sound = get(id);
		if (not sound)
		{
			return false;
		}

		return do_play_sound(sound, true);
	}

	auto Sound::play_unique(const std::string_view path) noexcept -> bool
	{
		const auto sound = get(path);
		if (not sound)
		{
			return false;
		}

		return do_play_sound(sound, true);
	}

	namespace
	{
		[[nodiscard]] auto get_music_channel() noexcept -> sf::Music*&
		{
			static sf::Music* music_channel{nullptr};
			return music_channel;
		}

		auto do_play_music(const music_type& music, const bool restart, const bool loop) noexcept -> bool
		{
			auto& channel = get_music_channel();

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

			channel->setLooping(loop);
			channel->setVolume(var::music_volume);
			music->play();

			return true;
		}

		auto do_stop_music(const music_type& music) noexcept -> bool
		{
			if (music->getStatus() != sf::Music::Status::Playing)
			{
				return true;
			}

			music->stop();

			auto& channel = get_music_channel();
			channel = nullptr;

			return true;
		}
	}

	auto Music::play(const AssetId id, const bool loop) noexcept -> bool
	{
		const auto music = get(id);
		if (not music)
		{
			return false;
		}

		return do_play_music(music, false, loop);
	}

	auto Music::play(const std::string_view path, const bool loop) noexcept -> bool
	{
		const auto music = get(path);
		if (not music)
		{
			return false;
		}

		return do_play_music(music, false, loop);
	}

	auto Music::play_or_restart(const AssetId id, const bool loop) noexcept -> bool
	{
		const auto music = get(id);
		if (not music)
		{
			return false;
		}

		return do_play_music(music, true, loop);
	}

	auto Music::play_or_restart(const std::string_view path, const bool loop) noexcept -> bool
	{
		const auto music = get(path);
		if (not music)
		{
			return false;
		}

		return do_play_music(music, true, loop);
	}

	auto Music::stop(const AssetId id) noexcept -> bool
	{
		const auto music = get(id);
		if (not music)
		{
			return false;
		}

		return do_stop_music(music);
	}

	auto Music::stop(const std::string_view path) noexcept -> bool
	{
		const auto music = get(path);
		if (not music)
		{
			return false;
		}

		return do_stop_music(music);
	}

	auto Music::stop_all() noexcept -> bool
	{
		auto& channel = get_music_channel();
		if (channel == nullptr)
		{
			return false;
		}

		channel->stop();
		return true;
	}
}
