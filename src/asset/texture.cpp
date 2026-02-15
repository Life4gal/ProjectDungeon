// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <asset/texture.hpp>

#include <spdlog/spdlog.h>
#include <SFML/Graphics/Texture.hpp>

namespace pd::asset
{
	auto TextureLoader::operator()(const std::filesystem::path& path) noexcept -> result_type
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
}
