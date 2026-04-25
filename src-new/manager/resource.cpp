// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <manager/resource.hpp>

#include <spdlog/spdlog.h>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>

namespace pd::manager::resource_detail
{
	auto FontLoader::operator()(const std::string_view path) const noexcept -> entt::resource<element_type>
	{
		return operator()(std::filesystem::path{path});
	}

	auto FontLoader::operator()(const std::filesystem::path& path) const noexcept -> entt::resource<element_type>
	{
		try
		{
			if (not exists(path))
			{
				SPDLOG_ERROR("载入字体[{:<20}]失败!文件不存在!", path.string());
				return entt::resource<element_type>{};
			}

			auto font = std::make_shared<sf::Font>();
			if (not font->openFromFile(path))
			{
				SPDLOG_ERROR("载入字体[{:<20}]失败!", path.string());
				return entt::resource<element_type>{};
			}

			SPDLOG_INFO("载入字体[{:<20}]成功!", path.string());

			return entt::resource{std::move(font)};
		}
		catch (const std::system_error& e)
		{
			SPDLOG_ERROR("载入字体[{:<20}]失败: {}", path.string(), e.what());
			return entt::resource<element_type>{};
		}
	}

	auto TextureLoader::operator()(const std::string_view path) const noexcept -> entt::resource<element_type>
	{
		return operator()(std::filesystem::path{path});
	}

	auto TextureLoader::operator()(const std::filesystem::path& path) const noexcept -> entt::resource<element_type>
	{
		try
		{
			if (not exists(path))
			{
				SPDLOG_ERROR("载入纹理[{:<20}]失败!文件不存在!", path.string());
				return entt::resource<element_type>{};
			}

			auto texture = std::make_shared<sf::Texture>();
			if (not texture->loadFromFile(path))
			{
				SPDLOG_ERROR("载入纹理[{:<20}]失败!", path.string());
				return entt::resource<element_type>{};
			}

			SPDLOG_INFO("载入纹理[{:<20}]成功!", path.string());

			return entt::resource{std::move(texture)};
		}
		catch (const std::system_error& e)
		{
			SPDLOG_ERROR("载入纹理[{:<20}]失败: {}", path.string(), e.what());
			return entt::resource<element_type>{};
		}
	}

	auto SoundLoader::operator()(const std::string_view path) const noexcept -> entt::resource<element_type>
	{
		return operator()(std::filesystem::path{path});
	}

	auto SoundLoader::operator()(const std::filesystem::path& path) const noexcept -> entt::resource<element_type>
	{
		try
		{
			if (not exists(path))
			{
				SPDLOG_ERROR("载入音效[{:<20}]失败!文件不存在!", path.string());
				return entt::resource<element_type>{};
			}

			auto sound = std::make_shared<sf::SoundBuffer>();
			if (not sound->loadFromFile(path))
			{
				SPDLOG_ERROR("载入音效[{:<20}]失败!", path.string());
				return entt::resource<element_type>{};
			}

			SPDLOG_INFO("载入音效[{:<20}]成功!", path.string());

			return entt::resource{std::move(sound)};
		}
		catch (const std::system_error& e)
		{
			SPDLOG_ERROR("载入音效[{:<20}]失败: {}", path.string(), e.what());
			return entt::resource<element_type>{};
		}
	}

	auto MusicLoader::operator()(const std::string_view path) const noexcept -> entt::resource<element_type>
	{
		return operator()(std::filesystem::path{path});
	}

	auto MusicLoader::operator()(const std::filesystem::path& path) const noexcept -> entt::resource<element_type>
	{
		try
		{
			if (not exists(path))
			{
				SPDLOG_ERROR("载入音乐[{:<20}]失败!文件不存在!", path.string());
				return entt::resource<element_type>{};
			}

			auto music = std::make_shared<sf::Music>();
			if (not music->openFromFile(path))
			{
				SPDLOG_ERROR("载入音乐[{:<20}]失败!", path.string());
				return entt::resource<element_type>{};
			}

			SPDLOG_INFO("载入音乐[{:<20}]成功!", path.string());

			return entt::resource{std::move(music)};
		}
		catch (const std::system_error& e)
		{
			SPDLOG_ERROR("载入音乐[{:<20}]失败: {}", path.string(), e.what());
			return entt::resource<element_type>{};
		}
	}
}
