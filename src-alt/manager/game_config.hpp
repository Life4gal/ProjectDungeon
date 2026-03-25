// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>

namespace pd::manager
{
	class GameConfig final
	{
		// 游戏窗口的宽度
		// 仅在窗口大小更新时修改
		unsigned window_width_;
		// 游戏窗口的高度
		// 仅在窗口大小更新时修改
		unsigned window_height_;

		// 音效音量
		float sound_volume_;
		// 音乐音量
		float music_volume_;

		// 这决定我们最多能创建多少个sf::Sound实例
		// 这部分资源基本不会释放 :(
		std::uint32_t max_sound_channels_;

		std::string player_name_;

		GameConfig() noexcept;

	public:
		// 这个接口公开仅为了Game::debug_game_config_
		[[nodiscard]] static auto instance() noexcept -> GameConfig&;

		[[nodiscard]] static auto get_window_width() noexcept -> unsigned
		{
			return instance().window_width_;
		}

		static auto set_window_width(const unsigned width) noexcept -> void
		{
			instance().window_width_ = width;
		}

		[[nodiscard]] static auto get_window_height() noexcept -> unsigned
		{
			return instance().window_height_;
		}

		static auto set_window_height(const unsigned height) noexcept -> void
		{
			instance().window_height_ = height;
		}

		[[nodiscard]] static auto get_sound_volume() noexcept -> float
		{
			return instance().sound_volume_;
		}

		static auto set_sound_volume(const float volume) noexcept -> void
		{
			instance().sound_volume_ = volume;
		}

		[[nodiscard]] static auto get_music_volume() noexcept -> float
		{
			return instance().music_volume_;
		}

		static auto set_music_volume(const float volume) noexcept -> void
		{
			instance().music_volume_ = volume;
		}

		[[nodiscard]] static auto get_max_sound_channels() noexcept -> std::uint32_t
		{
			return instance().max_sound_channels_;
		}

		static auto set_max_sound_channels(const std::uint32_t channels) noexcept -> void
		{
			instance().max_sound_channels_ = channels;
		}

		[[nodiscard]] static auto get_player_name() noexcept -> std::string_view
		{
			return instance().player_name_;
		}

		static auto set_player_name(const std::string_view name) noexcept -> void
		{
			instance().player_name_ = name;
		}
	};
}
