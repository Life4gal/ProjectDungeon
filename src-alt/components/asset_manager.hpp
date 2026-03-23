// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <vector>

#include <manager/asset_manager.hpp>

// 在这里引入SFML是可以接受的 :)
#include <SFML/Audio/Sound.hpp>

namespace sf
{
	class Music;
}

namespace pd::components
{
	// ==========================================
	// 上下文组件
	// ==========================================

	// 下面这几个manager可以直接用using,也可以用entt::hashed_string,不过我觉得这样更清晰一些 :)

	class FontManager final : public manager::FontManager {};

	class TextureManager final : public manager::TextureManager {};

	class SoundManager final : public manager::SoundManager {};

	class MusicManager final : public manager::MusicManager {};

	// 音效播放通道
	class SoundChannels final
	{
	public:
		std::vector<sf::Sound> channels;
	};

	// 音乐播放通道
	class MusicChannel final
	{
	public:
		sf::Music* music;
	};

	// ==========================================
	// 实体组件
	// ==========================================
}
