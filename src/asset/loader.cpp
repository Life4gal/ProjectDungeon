// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <asset/loader.hpp>

#include <spdlog/spdlog.h>

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>

namespace pd::asset
{
	auto FontLoader::operator()(const path_type& path) noexcept -> result_type
	{
		const auto s = path.string();

		if (not exists(path))
		{
			SPDLOG_ERROR("载入字体[{:<20}]失败!文件不存在!", s);
			return result_type{};
		}

		auto font = std::make_shared<sf::Font>();
		if (not font->openFromFile(path))
		{
			SPDLOG_ERROR("载入字体[{:<20}]失败!", s);
			return result_type{};
		}

		SPDLOG_INFO("载入字体[{:<20}]成功!", s);

		return font;
	}

	TextureLoader::result_type TextureLoader::operator()(const path_type& path) noexcept
	{
		const auto s = path.string();

		if (not exists(path))
		{
			SPDLOG_ERROR("载入纹理[{:<20}]失败!文件不存在!", s);
			return result_type{};
		}

		auto texture = std::make_shared<sf::Texture>();
		if (not texture->loadFromFile(path))
		{
			SPDLOG_ERROR("载入纹理[{:<20}]失败!", s);
			return result_type{};
		}

		SPDLOG_INFO("载入纹理[{:<20}]成功!", s);

		return texture;
	}

	auto SoundLoader::operator()(const path_type& path) noexcept -> result_type
	{
		const auto s = path.string();

		if (not exists(path))
		{
			SPDLOG_ERROR("载入音效[{:<20}]失败!文件不存在!", s);
			return result_type{};
		}

		auto sound = std::make_shared<sf::SoundBuffer>();
		if (not sound->loadFromFile(path))
		{
			SPDLOG_ERROR("载入音效[{:<20}]失败!", s);
			return result_type{};
		}

		SPDLOG_INFO("载入音效[{:<20}]成功!", s);

		return sound;
	}

	auto MusicLoader::operator()(const path_type& path) noexcept -> result_type
	{
		const auto s = path.string();

		if (not exists(path))
		{
			SPDLOG_ERROR("载入音乐[{:<20}]失败!文件不存在!", s);
			return result_type{};
		}

		auto music = std::make_shared<sf::Music>();
		if (not music->openFromFile(path))
		{
			SPDLOG_ERROR("载入音乐[{:<20}]失败!", s);
			return result_type{};
		}

		SPDLOG_INFO("载入音乐[{:<20}]成功!", s);

		return music;
	}
}
