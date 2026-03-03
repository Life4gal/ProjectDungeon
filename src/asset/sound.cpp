// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <asset/sound.hpp>

#include <filesystem>

#include <spdlog/spdlog.h>
#include <SFML/Audio/SoundBuffer.hpp>

namespace pd::asset
{
	auto SoundLoader::operator()(const std::string_view path) noexcept -> result_type
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
}
