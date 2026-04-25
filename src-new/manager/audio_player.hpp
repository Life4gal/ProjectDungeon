// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <manager/resource_fwd.hpp>

namespace pd::manager
{
	class AudioPlayer final
	{
	public:
		// ===================================================
		// 音效
		// ===================================================

		// 播放一个音效
		// false:
		//  -指定的音效加载失败(InvalidHandler)
		//  -指定的音效正在播放
		//  -不存在空闲音效通道
		static auto play(const sound_handler& sound) noexcept -> bool;

		// 播放一个音效,如果指定音效正在播放则跳过
		// false:
		//  -指定的音效加载失败(InvalidHandler)
		//  -指定的音效未加载
		//  -不存在空闲音效通道
		static auto play_unique(const sound_handler& sound) noexcept -> bool;

		// 清理所有音效通道(同时停止它们)
		static auto clear_sounds() noexcept -> void;

		// ===================================================
		// 音乐
		// ===================================================

		// 播放一个音乐,如果音乐正在播放则跳过
		// false:
		//  -指定的音乐加载失败(InvalidHandler)
		//  -指定的音乐正在播放
		static auto play(const music_handler& music, bool loop = true) noexcept -> bool;

		// 播放一个音乐,如果音乐正在播放则重启
		// false:
		//  -指定的音效加载失败(InvalidHandler)
		static auto play_force(const music_handler& music, bool loop = true) noexcept -> bool;

		// 暂停一个音乐
		// false:
		//  -指定的音乐加载失败(InvalidHandler)
		static auto pause(const music_handler& music) noexcept -> bool;

		// 恢复一个音乐
		// false:
		//  -指定的音乐加载失败(InvalidHandler)
		static auto unpause(const music_handler& music) noexcept -> bool;

		// 停止一个音乐
		// false:
		//  -指定的音乐加载失败(InvalidHandler)
		static auto stop(const music_handler& music) noexcept -> bool;

		// 清理所有音乐通道(同时停止它们)
		static auto clear_musics() noexcept -> void;
	};
}
