// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <asset/font.hpp>

#include <filesystem>

#include <spdlog/spdlog.h>
#include <SFML/Graphics/Font.hpp>

namespace pd::asset
{
	auto FontLoader::operator()(const std::string_view path) noexcept -> result_type
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
}
