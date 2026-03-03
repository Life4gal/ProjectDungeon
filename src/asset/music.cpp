// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <asset/music.hpp>

#include <filesystem>

#include <SFML/Audio/Music.hpp>
#include <spdlog/spdlog.h>

namespace pd::asset
{
	auto MusicLoader::operator()(const std::string_view path) noexcept -> result_type
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
} // namespace pd::asset
