// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <asset/texture.hpp>

#include <filesystem>

#include <spdlog/spdlog.h>
#include <SFML/Graphics/Texture.hpp>

namespace pd::asset
{
	auto TextureLoader::operator()(const std::string_view path) noexcept -> result_type
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
}
