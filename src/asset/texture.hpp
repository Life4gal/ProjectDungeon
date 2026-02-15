// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <filesystem>

#include <asset/resource_cache.hpp>

namespace sf
{
	class Texture;
}

namespace pd::asset
{
	class TextureLoader final
	{
	public:
		using result_type = std::shared_ptr<sf::Texture>;

		[[nodiscard]] static auto operator()(const std::filesystem::path& path) noexcept -> result_type;
	};

	class TextureManager final : public ResourceCache<TextureLoader> {};
} // namespace pd::asset
