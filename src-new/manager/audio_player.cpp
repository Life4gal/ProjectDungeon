// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <manager/audio_player.hpp>

#include <algorithm>
#include <vector>

#include <var/audio.hpp>

#include <manager/resource.hpp>

#include <spdlog/spdlog.h>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>

namespace pd::manager
{
	namespace
	{
		std::vector<sf::Sound> sound_channels{};
		std::vector<std::weak_ptr<sf::Music>> music_channels{};

		[[nodiscard]] auto do_play_sound(const sound_handler& sound, const bool unique) noexcept -> bool
		{
			if (sound == InvalidHandler)
			{
				SPDLOG_WARN("播放失败: 音效无效!请检查[{}]是否加载成功!", Sound::path_of(sound));
				return false;
			}

			if (unique)
			{
				// 如果存在还在播放的相同声音则跳过
				const auto stopped_channel_it = std::ranges::find_if(
					sound_channels,
					[&sound](const auto& channel) noexcept -> bool
					{
						return channel.getStatus() != sf::Sound::Status::Stopped and std::addressof(channel.getBuffer()) == sound.operator->();
					}
				);
				if (stopped_channel_it != sound_channels.end())
				{
					return false;
				}
			}

			// 寻找一个空闲通道来播放音效
			if (const auto free_channel_it = std::ranges::find(sound_channels, sf::Sound::Status::Stopped, &sf::Sound::getStatus);
				// 如果找到空闲通道则使用它来播放音效
				free_channel_it != sound_channels.end())
			{
				auto& channel = *free_channel_it;

				channel.setBuffer(*sound);
				channel.setVolume(var::sound_volume);
				channel.play();

				return true;
			}

			// 如果当前通道数量已经超过最大值则跳过
			if (sound_channels.size() > var::max_sound_channels)
			{
				SPDLOG_WARN("播放失败: 所有音效通道正忙,丢弃音效[{}]...", Sound::path_of(sound));
				return false;
			}

			auto& channel = sound_channels.emplace_back(*sound);
			channel.setVolume(var::sound_volume);
			channel.play();

			return true;
		}

		[[nodiscard]] auto do_play_music(const music_handler& music, const bool force, const bool loop) noexcept -> bool
		{
			if (music == InvalidHandler)
			{
				SPDLOG_WARN("播放失败: 音乐无效!请检查[{}]是否加载成功!", Music::path_of(music));
				return false;
			}

			// 检查是否已存在
			const auto exist_channel_it = std::ranges::find_if(
				music_channels,
				[&music](const auto& channel) noexcept -> bool
				{
					if (channel.expired())
					{
						return false;
					}

					const auto m = channel.lock();
					return m == music.handle();
				}
			);

			// 如果已存在
			if (exist_channel_it != music_channels.end())
			{
				if (not force)
				{
					// 如果不强制则什么也不做
				}
				else
				{
					music->stop();
					music->setVolume(var::music_volume);
					music->play();
				}

				music->setLooping(loop);

				return true;
			}

			// 如果不存在

			// 寻找一个空闲通道来播放音乐
			if (const auto free_channel_it = std::ranges::find(music_channels, true, &std::weak_ptr<sf::Music>::expired);
				// 如果找到空闲通道则使用它来播放音乐
				free_channel_it != music_channels.end())
			{
				*free_channel_it = music.handle();

				music->setLooping(loop);
				music->setVolume(var::music_volume);
				music->play();

				return true;
			}

			// 如果当前通道数量已经超过最大值则跳过
			if (music_channels.size() > var::max_music_channels)
			{
				SPDLOG_WARN("播放失败: 所有音乐通道正忙,丢弃音乐[{}]...", Music::path_of(music));
				return false;
			}

			music_channels.emplace_back(music.handle());
			music->setLooping(loop);
			music->setVolume(var::music_volume);
			music->play();

			return true;
		}
	}

	auto AudioPlayer::play(const sound_handler& sound) noexcept -> bool
	{
		return do_play_sound(sound, false);
	}

	auto AudioPlayer::play_unique(const sound_handler& sound) noexcept -> bool
	{
		return do_play_sound(sound, true);
	}

	auto AudioPlayer::clear_sounds() noexcept -> void
	{
		std::ranges::for_each(sound_channels, &sf::Sound::stop);
		sound_channels.clear();
	}

	auto AudioPlayer::play(const music_handler& music, const bool loop) noexcept -> bool
	{
		return do_play_music(music, false, loop);
	}

	auto AudioPlayer::play_force(const music_handler& music, const bool loop) noexcept -> bool
	{
		return do_play_music(music, true, loop);
	}

	auto AudioPlayer::pause(const music_handler& music) noexcept -> bool
	{
		if (music == InvalidHandler)
		{
			SPDLOG_WARN("暂停失败: 音乐无效!请检查[{}]是否加载成功!", Music::path_of(music));
			return false;
		}

		music->pause();
		return true;
	}

	auto AudioPlayer::unpause(const music_handler& music) noexcept -> bool
	{
		if (music == InvalidHandler)
		{
			SPDLOG_WARN("恢复失败: 音乐无效!请检查[{}]是否加载成功!", Music::path_of(music));
			return false;
		}

		music->play();
		return true;
	}

	auto AudioPlayer::stop(const music_handler& music) noexcept -> bool
	{
		if (music == InvalidHandler)
		{
			SPDLOG_WARN("停止失败: 音乐无效!请检查[{}]是否加载成功!", Music::path_of(music));
			return false;
		}

		// 仅停止音乐,不清理通道
		music->stop();
		return true;
	}

	auto AudioPlayer::clear_musics() noexcept -> void {}
}
