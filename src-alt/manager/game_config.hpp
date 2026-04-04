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
	public:
		class Vars final
		{
		public:
			// 游戏窗口的宽度
			// 仅在窗口大小更新时修改
			unsigned window_width;
			// 游戏窗口的高度
			// 仅在窗口大小更新时修改
			unsigned window_height;

			// 音效音量
			float sound_volume;
			// 音乐音量
			float music_volume;

			// 这决定我们最多能创建多少个sf::Sound实例
			// 这部分资源基本不会释放 :(
			std::uint32_t max_sound_channels;

			// 玩家名称
			std::string player_name;
		};

	private:
		inline static Vars vars_
		{
				.window_width = 1280,
				.window_height = 720,
				.sound_volume = 100,
				.music_volume = 100,
				.max_sound_channels = 16,
				.player_name = "Unnamed",
		};

	public:
		// [[nodiscard]] static auto instance() noexcept -> Vars&
		// {
		// 	return vars_;
		// }

		[[nodiscard]] static auto get_window_width() noexcept -> unsigned
		{
			return vars_.window_width;
		}

		static auto set_window_width(const unsigned width) noexcept -> void
		{
			vars_.window_width = width;
		}

		[[nodiscard]] static auto get_window_height() noexcept -> unsigned
		{
			return vars_.window_height;
		}

		static auto set_window_height(const unsigned height) noexcept -> void
		{
			vars_.window_height = height;
		}

		[[nodiscard]] static auto get_sound_volume() noexcept -> float
		{
			return vars_.sound_volume;
		}

		static auto set_sound_volume(const float volume) noexcept -> void
		{
			vars_.sound_volume = volume;
		}

		[[nodiscard]] static auto get_music_volume() noexcept -> float
		{
			return vars_.music_volume;
		}

		static auto set_music_volume(const float volume) noexcept -> void
		{
			vars_.music_volume = volume;
		}

		[[nodiscard]] static auto get_max_sound_channels() noexcept -> std::uint32_t
		{
			return vars_.max_sound_channels;
		}

		static auto set_max_sound_channels(const std::uint32_t channels) noexcept -> void
		{
			vars_.max_sound_channels = channels;
		}

		[[nodiscard]] static auto get_player_name() noexcept -> std::string_view
		{
			return vars_.player_name;
		}

		static auto set_player_name(const std::string_view name) noexcept -> void
		{
			vars_.player_name = name;
		}
	};
}
