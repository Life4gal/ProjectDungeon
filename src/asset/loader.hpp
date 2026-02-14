// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <asset/map.hpp>

#include <entt/resource/cache.hpp>

namespace sf
{
	class Font;
	class Texture;
	class SoundBuffer;
	class Music;
}

namespace pd::asset
{
	class FontLoader final
	{
	public:
		using result_type = std::shared_ptr<sf::Font>;

		[[nodiscard]] static auto operator()(const path_type& path) noexcept -> result_type;
	};

	class TextureLoader final
	{
	public:
		using result_type = std::shared_ptr<sf::Texture>;

		[[nodiscard]] static auto operator()(const path_type& path) noexcept -> result_type;
	};

	class SoundLoader final
	{
	public:
		using result_type = std::shared_ptr<sf::SoundBuffer>;

		[[nodiscard]] static auto operator()(const path_type& path) noexcept -> result_type;
	};

	class MusicLoader final
	{
	public:
		using result_type = std::shared_ptr<sf::Music>;

		[[nodiscard]] static auto operator()(const path_type& path) noexcept -> result_type;
	};
}
