// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <asset/music.hpp>

#include <SFML/Audio/Music.hpp>
#include <spdlog/spdlog.h>

namespace pd::asset
{
	auto MusicLoader::operator()(const std::filesystem::path& path) noexcept -> result_type
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
} // namespace pd::asset
