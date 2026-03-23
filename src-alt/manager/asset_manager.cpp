// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <manager/asset_manager.hpp>

#include <spdlog/spdlog.h>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>

namespace pd::manager::detail
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
