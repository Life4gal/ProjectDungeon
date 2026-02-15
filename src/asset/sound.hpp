// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>

#include <asset/resource_cache.hpp>

namespace sf
{
	class SoundBuffer;
}

namespace pd::asset
{
	class SoundLoader final
	{
	public:
		using result_type = std::shared_ptr<sf::SoundBuffer>;

		[[nodiscard]] static auto operator()(const std::filesystem::path& path) noexcept -> result_type;
	};

	class SoundManager final : public ResourceCache<SoundLoader> {};
}
